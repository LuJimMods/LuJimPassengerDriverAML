#include "PassengerDriver.h"
#include "Config.h"
#include "Input.h"
#include "Notifications.h"
#include "Log.h"
#include "Recruit.h"
#include "Vehicle.h"
#include "GameSymbols.h"

#include <cstdint>

namespace
{
    FindPlayerPedFn gFindPlayerPed = nullptr;
    FindPlayerVehicleFn gFindPlayerVehicle = nullptr;
    VehicleIsDriverFn gVehicleIsDriver = nullptr;

    bool gSymbolsResolved = false;
    bool gSymbolsLogged = false;

    void* gLastPed = nullptr;
    void* gLastVehicle = nullptr;
    void* gLastPassengerVehicle = nullptr;

    int gLastSeatState = -999;
    int gPassengerStableTicks = 0;

    bool gPlayerWasKnown = false;
    bool gVehicleWasKnown = false;
    bool gPassengerModeLogged = false;

    unsigned int gMonitorTick = 0;

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
            LPD_Log("[MONITOR] V2.6.0 simbolos: FindPlayerPed=%p FindPlayerVehicle=%p CVehicle::IsDriver=%p",
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
        gLastSeatState = 0;
        gPassengerStableTicks = 0;
        gLastPassengerVehicle = nullptr;
        gPassengerModeLogged = false;
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
            LPD_Log("[PASSENGER] V2.6.0 apenas detecta. Nao move jogador e nao controla recruta.");
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
            return;
        }

        if(!gPlayerWasKnown || ped != gLastPed)
        {
            LPD_Log("[PLAYER] CJ encontrado. Ped=%p", ped);
            gPlayerWasKnown = true;
            gLastPed = ped;
        }

        void* vehicle = nullptr;
        if(gFindPlayerVehicle)
        {
            vehicle = gFindPlayerVehicle(-1, false);
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
            LPD_Log("[MONITOR] V2.6.0 ativo. Ped=%p Vehicle=%p Seat=%s PassengerTicks=%d Enabled=%d ExperimentalHooks=%d",
                    ped,
                    vehicle,
                    SeatName(seatState),
                    gPassengerStableTicks,
                    LPDSettings::values.enabled ? 1 : 0,
                    LPDSettings::values.experimentalHooks ? 1 : 0);
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

    if(LPDSettings::values.enabled)
    {
        LPD_Log("[INIT] Modo salvo no INI: ativado.");
    }
    else
    {
        LPD_Log("[INIT] Modo salvo no INI: desativado.");
    }

    LPD_Log("[INIT] V2.6.0: detecta banco de passageiro. Nao move jogador e nao controla recruta.");
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

    LPD_Log("[STATE] Enabled=%d", LPDSettings::values.enabled ? 1 : 0);
}

void PassengerDriver::Update(float dtMs)
{
    Input::Update(dtMs);

    if(Input::ConsumeToggleRequest())
    {
        Toggle();
    }

    // V2.6.0: monitoramento seguro sempre ativo para teste.
    // Ele apenas detecta motorista/passageiro e grava no log.
    MonitorPlayerVehicle();

    if(!LPDSettings::values.enabled)
    {
        return;
    }

    if(!LPDSettings::values.experimentalHooks)
    {
        static bool warned = false;
        if(!warned)
        {
            LPD_Log("[SAFE] ExperimentalHooks=0. V2.6.0 apenas detecta passageiro. Acao real bloqueada para evitar crash.");
            warned = true;
        }
        return;
    }

    // Etapa experimental futura: aqui vamos detectar veiculo/recruta e iniciar controle real.
    void* veh = Vehicle::FindTargetVehicle();
    void* rec = Recruit::FindValidRecruit();

    if(!veh || !rec)
    {
        static bool warnedMissing = false;
        if(!warnedMissing)
        {
            Notifications::NoRecruit();
            LPD_Log("[MONITOR] Nenhum veiculo/recruta valido detectado no modo experimental.");
            warnedMissing = true;
        }
        return;
    }
}
