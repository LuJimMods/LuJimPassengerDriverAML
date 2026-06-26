#include "PassengerDriver.h"
#include "Config.h"
#include "Input.h"
#include "Notifications.h"
#include "Log.h"
#include "Recruit.h"
#include "Vehicle.h"
#include "GameSymbols.h"

#include <mod/amlmod.h>
#include <cstdint>
#include <cstring>

namespace
{
    FindPlayerPedFn gFindPlayerPed = nullptr;
    FindPlayerVehicleFn gFindPlayerVehicle = nullptr;
    VehicleIsDriverFn gVehicleIsDriver = nullptr;

    bool gSymbolsResolved = false;
    bool gSymbolsLogged = false;
    bool gHooksTried = false;
    bool gHooksInstalled = false;

    void* gLastPed = nullptr;
    void* gLastVehicle = nullptr;
    void* gLastCandidateVehicle = nullptr;
    void* gLastPassengerVehicle = nullptr;

    int gLastSeatState = -999;
    int gPassengerStableTicks = 0;

    bool gPlayerWasKnown = false;
    bool gVehicleWasKnown = false;
    bool gPassengerModeLogged = false;
    bool gEntryCandidateLogged = false;
    bool gEntryCandidateActive = false;
    bool gForcedDriverLogged = false;
    void* gEntryCandidateVehicle = nullptr;
    int gEntryCandidateTicks = 0;

    unsigned int gMonitorTick = 0;
    unsigned int gHookEventCount = 0;
    void* gLastHookPed = nullptr;
    void* gLastHookThis = nullptr;
    void* gLastTaskVehicleOffset10 = nullptr;
    unsigned int gLastTaskSeatOrDoorOffset14 = 0;
    unsigned int gLastTaskFlagsOffset24 = 0;
    unsigned int gForceDoorWriteCount = 0;

    const uintptr_t OFF_CTaskComplexEnterCar_CreateFirstSubTask = 0x004F4C75;
    const uintptr_t OFF_CTaskComplexEnterAnyCarAsDriver_CreateFirstSubTask = 0x004FC985;
    const uintptr_t OFF_CTaskSimpleCarOpenDoorFromOutside_ProcessPed = 0x00500CBD;

    using CreateFirstSubTaskFn = void* (*)(void* task, void* ped);
    using ProcessPedFn = bool (*)(void* task, void* ped);

    CreateFirstSubTaskFn orgEnterCarCreateFirst = nullptr;
    CreateFirstSubTaskFn orgEnterAnyCarAsDriverCreateFirst = nullptr;
    ProcessPedFn orgOpenDoorProcessPed = nullptr;

    const char* SeatName(int state)
    {
        switch(state)
        {
            case 0: return "Fora do veiculo";
            case 1: return "Motorista";
            case 2: return "Passageiro";
            case 3: return "Dentro do veiculo";
            default: return "Desconhecido";
        }
    }

    void ResolveMonitorSymbols()
    {
        if(gSymbolsResolved) return;
        gSymbolsResolved = true;

        gFindPlayerPed = GetFindPlayerPed();
        gFindPlayerVehicle = GetFindPlayerVehicle();
        gVehicleIsDriver = GetVehicleIsDriver();

        if(!gSymbolsLogged)
        {
            LPD_Log("[MONITOR] V5.1.0 simbolos: FindPlayerPed=%p FindPlayerVehicle=%p CVehicle::IsDriver=%p",
                    reinterpret_cast<void*>(gFindPlayerPed),
                    reinterpret_cast<void*>(gFindPlayerVehicle),
                    reinterpret_cast<void*>(gVehicleIsDriver));
            gSymbolsLogged = true;
        }
    }

    void ResetVehicleState()
    {
        gVehicleWasKnown = false;
        gLastVehicle = nullptr;
        gLastCandidateVehicle = nullptr;
        gLastSeatState = 0;
        gPassengerStableTicks = 0;
        gLastPassengerVehicle = nullptr;
        gPassengerModeLogged = false;
    }

    void ResetEntryCandidate()
    {
        gEntryCandidateActive = false;
        gEntryCandidateVehicle = nullptr;
        gEntryCandidateTicks = 0;
        gEntryCandidateLogged = false;
        gForcedDriverLogged = false;
    }

    bool LooksLikeValidPtr(uintptr_t value)
    {
        return value >= 0x90000000u && value <= 0xBFFFFFFFu;
    }

    void DumpTaskWords(const char* hookName, void* task, void* ped)
    {
        if(!task) return;

        // Dump controlado: apenas nos primeiros eventos importantes para nao lotar o log.
        if(gHookEventCount > 12 && (gHookEventCount % 40) != 1) return;

        uintptr_t* w = reinterpret_cast<uintptr_t*>(task);

        void* currentVehicle = nullptr;
        void* remoteVehicle = nullptr;
        if(gFindPlayerVehicle)
        {
            currentVehicle = gFindPlayerVehicle(-1, false);
            remoteVehicle = gFindPlayerVehicle(-1, true);
        }

        LPD_Log("[TASK-DUMP] V5.1.0 %s Task=%p Ped=%p CurrentVehicle=%p CandidateVehicle=%p",
                hookName, task, ped, currentVehicle, remoteVehicle);

        LPD_Log("[TASK-DUMP] +00=%08x +04=%08x +08=%08x +0C=%08x",
                (unsigned int)w[0], (unsigned int)w[1], (unsigned int)w[2], (unsigned int)w[3]);
        LPD_Log("[TASK-DUMP] +10=%08x +14=%08x +18=%08x +1C=%08x",
                (unsigned int)w[4], (unsigned int)w[5], (unsigned int)w[6], (unsigned int)w[7]);
        LPD_Log("[TASK-DUMP] +20=%08x +24=%08x +28=%08x +2C=%08x",
                (unsigned int)w[8], (unsigned int)w[9], (unsigned int)w[10], (unsigned int)w[11]);
        LPD_Log("[TASK-DUMP] +30=%08x +34=%08x +38=%08x +3C=%08x",
                (unsigned int)w[12], (unsigned int)w[13], (unsigned int)w[14], (unsigned int)w[15]);

        for(int i = 0; i < 16; ++i)
        {
            if(LooksLikeValidPtr(w[i]))
            {
                LPD_Log("[TASK-FIELD] Possivel ponteiro em +0x%02X = %p", i * 4, (void*)w[i]);
            }
        }

        void* vehicleFromOffset10 = (void*)w[4];
        unsigned int seatOrDoorOffset14 = (unsigned int)w[5];
        unsigned int valueOffset18 = (unsigned int)w[6];
        unsigned int valueOffset24 = (unsigned int)w[9];

        gLastTaskVehicleOffset10 = LooksLikeValidPtr(w[4]) ? vehicleFromOffset10 : nullptr;
        gLastTaskSeatOrDoorOffset14 = seatOrDoorOffset14;
        gLastTaskFlagsOffset24 = valueOffset24;

        LPD_Log("[TASK-CHECK] V5.1.0 campos principais: +0x10(vehicle)=%p +0x14(seat/door?)=%u +0x18=%08x +0x24=%08x",
                vehicleFromOffset10,
                seatOrDoorOffset14,
                valueOffset18,
                valueOffset24);

        if(currentVehicle || remoteVehicle || gLastTaskVehicleOffset10)
        {
            LPD_Log("[TASK-CHECK] V5.1.0 comparacao: task+0x10=%p current=%p candidate=%p matchCurrent=%d matchCandidate=%d",
                    gLastTaskVehicleOffset10,
                    currentVehicle,
                    remoteVehicle,
                    (gLastTaskVehicleOffset10 && currentVehicle == gLastTaskVehicleOffset10) ? 1 : 0,
                    (gLastTaskVehicleOffset10 && remoteVehicle == gLastTaskVehicleOffset10) ? 1 : 0);
        }

        if(LPDSettings::values.enabled)
        {
            LPD_Log("[FORCE-PLAN] Enabled=1. V5.1.0 testa escrita segura em +0x14 para tentar passageiro.");
        }
    }


    void TryForcePassengerDoorField(const char* hookName, void* task, void* ped)
    {
        if(!LPDSettings::values.enabled) return;
        if(!task) return;

        // V5.1.0: teste controlado.
        // V5.0 provou que +0x14 da task simples controla a porta/lado da animacao,
        // mas nao o banco final. Agora tambem testamos +0x24 na task pai
        // (CTaskComplexEnterCar), que aparece como 6 nos dumps da task principal.
        if(!hookName || std::strstr(hookName, "CTaskSimpleCarOpenDoorFromOutside::ProcessPed") == nullptr)
        {
            return;
        }

        uintptr_t* w = reinterpret_cast<uintptr_t*>(task);
        void* parentTask = (void*)w[1];
        void* vehicleFromTask = (void*)w[4];

        if(!LooksLikeValidPtr(w[4]))
        {
            if((gForceDoorWriteCount % 20) == 0)
            {
                LPD_Log("[PASSENGER-FORCE] Ignorado: task+0x10 nao parece CVehicle. Task=%p Ped=%p +0x10=%p", task, ped, vehicleFromTask);
            }
            return;
        }

        unsigned int beforeDoor = (unsigned int)w[5];
        unsigned int afterDoor = 8;

        if(beforeDoor != 8 && beforeDoor != 10)
        {
            if((gForceDoorWriteCount % 20) == 0)
            {
                LPD_Log("[PASSENGER-FORCE] Ignorado: +0x14 inesperado. Task=%p Vehicle=%p +0x14=%u", task, vehicleFromTask, beforeDoor);
            }
            return;
        }

        w[5] = afterDoor;

        bool parentForced = false;
        unsigned int parentBefore24 = 0;
        unsigned int parentAfter24 = 8;

        if(LooksLikeValidPtr((uintptr_t)parentTask))
        {
            uintptr_t* p = reinterpret_cast<uintptr_t*>(parentTask);

            // Pela V4/V5, parent+0x0C costuma ser o CVehicle na task principal.
            bool parentVehicleMatches = ((void*)p[3] == vehicleFromTask);
            parentBefore24 = (unsigned int)p[9];

            // Só escreve se a task pai parecer correta e o valor for pequeno/esperado.
            if(parentVehicleMatches && (parentBefore24 == 6 || parentBefore24 == 8 || parentBefore24 == 10))
            {
                p[9] = parentAfter24;
                parentForced = true;
            }

            if(gForceDoorWriteCount <= 20 || (gForceDoorWriteCount % 20) == 0 || parentForced)
            {
                LPD_Log("[PARENT-FORCE] V5.1.0 parent=%p parent+0x0C=%p vehicle=%p match=%d +0x24 before=%u after=%u wrote=%d",
                        parentTask,
                        (void*)p[3],
                        vehicleFromTask,
                        parentVehicleMatches ? 1 : 0,
                        parentBefore24,
                        parentAfter24,
                        parentForced ? 1 : 0);
            }
        }

        ++gForceDoorWriteCount;
        gLastTaskVehicleOffset10 = vehicleFromTask;
        gLastTaskSeatOrDoorOffset14 = afterDoor;
        gLastTaskFlagsOffset24 = parentForced ? parentAfter24 : gLastTaskFlagsOffset24;

        if(gForceDoorWriteCount <= 20 || (gForceDoorWriteCount % 20) == 0 || beforeDoor != afterDoor || parentForced)
        {
            LPD_Log("[PASSENGER-FORCE] V5.1.0 escrito simple+0x14: before=%u after=%u Task=%p Parent=%p Ped=%p Vehicle=%p ParentForce=%d Count=%u",
                    beforeDoor,
                    afterDoor,
                    task,
                    parentTask,
                    ped,
                    vehicleFromTask,
                    parentForced ? 1 : 0,
                    gForceDoorWriteCount);
        }
    }

    void LogEntryHook(const char* hookName, void* task, void* ped)
    {
        ++gHookEventCount;

        TryForcePassengerDoorField(hookName, task, ped);

        bool importantChange = (ped != gLastHookPed) || (task != gLastHookThis);
        gLastHookPed = ped;
        gLastHookThis = task;

        if(importantChange || (gHookEventCount % 20) == 1)
        {
            LPD_Log("[HOOK-ENTRY] V5.1.0 %s chamado. Task=%p Ped=%p Enabled=%d ExperimentalHooks=%d Count=%u",
                    hookName,
                    task,
                    ped,
                    LPDSettings::values.enabled ? 1 : 0,
                    LPDSettings::values.experimentalHooks ? 1 : 0,
                    gHookEventCount);
            DumpTaskWords(hookName, task, ped);
        }
    }

    void* Hook_EnterCar_CreateFirstSubTask(void* task, void* ped)
    {
        LogEntryHook("CTaskComplexEnterCar::CreateFirstSubTask", task, ped);
        return orgEnterCarCreateFirst ? orgEnterCarCreateFirst(task, ped) : nullptr;
    }

    void* Hook_EnterAnyCarAsDriver_CreateFirstSubTask(void* task, void* ped)
    {
        LogEntryHook("CTaskComplexEnterAnyCarAsDriver::CreateFirstSubTask", task, ped);
        return orgEnterAnyCarAsDriverCreateFirst ? orgEnterAnyCarAsDriverCreateFirst(task, ped) : nullptr;
    }

    bool Hook_OpenDoor_ProcessPed(void* task, void* ped)
    {
        LogEntryHook("CTaskSimpleCarOpenDoorFromOutside::ProcessPed", task, ped);
        return orgOpenDoorProcessPed ? orgOpenDoorProcessPed(task, ped) : false;
    }

    void InstallEntryHooks()
    {
        if(gHooksTried) return;
        gHooksTried = true;

#ifndef AML32
        LPD_Log("[HOOK] V5.1.0 hooks de entrada ignorados: build nao e ARM32.");
        return;
#else
        if(!aml)
        {
            LPD_Log("[HOOK] V5.1.0 falha: AML interface nula.");
            return;
        }

        if(!gSymbols.base)
        {
            LPD_Log("[HOOK] V5.1.0 falha: base da libGTASA ainda nao encontrada.");
            return;
        }

        bool ok1 = aml->Hook((void*)GTASA(OFF_CTaskComplexEnterCar_CreateFirstSubTask),
                             (void*)Hook_EnterCar_CreateFirstSubTask,
                             (void**)&orgEnterCarCreateFirst);

        bool ok2 = aml->Hook((void*)GTASA(OFF_CTaskComplexEnterAnyCarAsDriver_CreateFirstSubTask),
                             (void*)Hook_EnterAnyCarAsDriver_CreateFirstSubTask,
                             (void**)&orgEnterAnyCarAsDriverCreateFirst);

        bool ok3 = aml->Hook((void*)GTASA(OFF_CTaskSimpleCarOpenDoorFromOutside_ProcessPed),
                             (void*)Hook_OpenDoor_ProcessPed,
                             (void**)&orgOpenDoorProcessPed);

        gHooksInstalled = ok1 || ok2 || ok3;

        LPD_Log("[HOOK] V5.1.0 Entry hooks instalados. ok1=%d ok2=%d ok3=%d base=0x%08x",
                ok1 ? 1 : 0,
                ok2 ? 1 : 0,
                ok3 ? 1 : 0,
                (unsigned int)gSymbols.base);

        LPD_Log("[HOOK] Enderecos: EnterCarCreateFirst=%p EnterAnyDriverCreateFirst=%p OpenDoorProcessPed=%p",
                GTASA(OFF_CTaskComplexEnterCar_CreateFirstSubTask),
                GTASA(OFF_CTaskComplexEnterAnyCarAsDriver_CreateFirstSubTask),
                GTASA(OFF_CTaskSimpleCarOpenDoorFromOutside_ProcessPed));
#endif
    }

    void LogPassengerDetection(void* ped, void* vehicle)
    {
        if(vehicle != gLastPassengerVehicle)
        {
            gLastPassengerVehicle = vehicle;
            gPassengerStableTicks = 0;
            gPassengerModeLogged = false;
        }

        ++gPassengerStableTicks;

        if(!gPassengerModeLogged)
        {
            LPD_Log("[PASSENGER] Entrada/estado como passageiro detectado. Ped=%p Vehicle=%p Ticks=%d",
                    ped,
                    vehicle,
                    gPassengerStableTicks);
            LPD_Log("[PASSENGER] V5.1.0 apenas detecta. Nao move jogador e nao controla recruta.");
            gPassengerModeLogged = true;
        }
    }

    void MonitorPlayerVehicle()
    {
        ResolveMonitorSymbols();

        if(!gFindPlayerPed)
        {
            static bool warnedPedSymbol = false;
            if(!warnedPedSymbol)
            {
                LPD_Log("[MONITOR] FindPlayerPed nao encontrado. Monitor de veiculo aguardando simbolo valido.");
                warnedPedSymbol = true;
            }
            return;
        }

        void* ped = gFindPlayerPed(-1);
        if(!ped)
        {
            if(gPlayerWasKnown)
            {
                LPD_Log("[PLAYER] CJ perdido/indisponivel");
            }

            gPlayerWasKnown = false;
            gLastPed = nullptr;
            ResetVehicleState();
            ResetEntryCandidate();
            return;
        }

        if(!gPlayerWasKnown || ped != gLastPed)
        {
            LPD_Log("[PLAYER] CJ encontrado. Ped=%p", ped);
            gPlayerWasKnown = true;
            gLastPed = ped;
        }

        void* vehicle = nullptr;
        void* candidateVehicle = nullptr;
        if(gFindPlayerVehicle)
        {
            vehicle = gFindPlayerVehicle(-1, false);
            candidateVehicle = gFindPlayerVehicle(-1, true);
        }
        else
        {
            static bool warnedVehicleSymbol = false;
            if(!warnedVehicleSymbol)
            {
                LPD_Log("[MONITOR] FindPlayerVehicle nao encontrado. Ainda nao e possivel detectar o veiculo atual.");
                warnedVehicleSymbol = true;
            }
        }

        if(candidateVehicle != gLastCandidateVehicle)
        {
            LPD_Log("[TARGET] V5.1.0 FindPlayerVehicle(includeRemote=true) mudou. CandidateVehicle=%p CurrentVehicle=%p",
                    candidateVehicle,
                    vehicle);

            if(gLastTaskVehicleOffset10 || candidateVehicle || vehicle)
            {
                LPD_Log("[TARGET-CHECK] V5.1.0 ultimo task+0x10=%p candidate=%p current=%p matchCandidate=%d matchCurrent=%d +0x14=%u +0x24=%08x",
                        gLastTaskVehicleOffset10,
                        candidateVehicle,
                        vehicle,
                        (gLastTaskVehicleOffset10 && candidateVehicle == gLastTaskVehicleOffset10) ? 1 : 0,
                        (gLastTaskVehicleOffset10 && vehicle == gLastTaskVehicleOffset10) ? 1 : 0,
                        gLastTaskSeatOrDoorOffset14,
                        gLastTaskFlagsOffset24);
            }

            gLastCandidateVehicle = candidateVehicle;
        }

        int seatState = 0;
        if(vehicle)
        {
            if(gVehicleIsDriver)
            {
                bool isDriver = gVehicleIsDriver(vehicle, ped);
                seatState = isDriver ? 1 : 2;
            }
            else
            {
                seatState = 3;
            }
        }

        bool changed = false;
        if(vehicle != gLastVehicle) changed = true;
        if(seatState != gLastSeatState) changed = true;

        if(changed)
        {
            if(vehicle)
            {
                LPD_Log("[PLAYER] Dentro de veiculo. Vehicle=%p Seat=%s", vehicle, SeatName(seatState));
                gVehicleWasKnown = true;
            }
            else
            {
                if(gVehicleWasKnown)
                {
                    LPD_Log("[PLAYER] Fora do veiculo");
                }
                ResetVehicleState();
            }

            gLastVehicle = vehicle;
            gLastSeatState = seatState;
        }

        if(vehicle && seatState == 2)
        {
            LogPassengerDetection(ped, vehicle);
        }
        else if(seatState != 2)
        {
            gPassengerStableTicks = 0;
            gLastPassengerVehicle = nullptr;
            gPassengerModeLogged = false;
        }

        ++gMonitorTick;
        if((gMonitorTick % 40) == 0)
        {
            LPD_Log("[MONITOR] V5.1.0 ativo. Ped=%p Vehicle=%p Candidate=%p Seat=%s HookEvents=%u HooksInstalled=%d Enabled=%d ExperimentalHooks=%d ForceWrites=%u",
                    ped,
                    vehicle,
                    candidateVehicle,
                    SeatName(seatState),
                    gHookEventCount,
                    gHooksInstalled ? 1 : 0,
                    LPDSettings::values.enabled ? 1 : 0,
                    LPDSettings::values.experimentalHooks ? 1 : 0,
                    gForceDoorWriteCount);
        }
    }
}

void PassengerDriver::Init()
{
    LPDSettings::Load();

    LPD_Log("[INIT] LuJim Passenger Driver iniciado. Enabled=%d HoldTime=%d ExperimentalHooks=%d",
            LPDSettings::values.enabled ? 1 : 0,
            LPDSettings::values.holdTimeMs,
            LPDSettings::values.experimentalHooks ? 1 : 0);

    LPD_Log("[INIT] Modo salvo no INI: %s.",
            LPDSettings::values.enabled ? "ativado" : "desativado");

    LPD_Log("[INIT] V5.1.0: teste real 2: força +0x14 da task de porta e +0x24 da task pai para procurar assento final.");
    InstallEntryHooks();
}

void PassengerDriver::Toggle()
{
    LPDSettings::SaveEnabled(!LPDSettings::values.enabled);

    if(LPDSettings::values.enabled)
    {
        Notifications::Enabled();
    }
    else
    {
        Notifications::Disabled();
    }

    ResetEntryCandidate();

    LPD_Log("[STATE] V5.1.0 Enabled=%d. Modo %s",
            LPDSettings::values.enabled ? 1 : 0,
            LPDSettings::values.enabled ? "ativado" : "desativado");
}

void PassengerDriver::Update(float dtMs)
{
    Input::Update(dtMs);

    if(Input::ConsumeToggleRequest())
    {
        Toggle();
    }

    MonitorPlayerVehicle();

    if(!LPDSettings::values.enabled)
    {
        return;
    }

    // V5.1.0: confirma os campos da task.
    // Ainda nao escreve memoria; proxima versao pode testar troca se +0x10 e +0x14 ficarem consistentes.
}
