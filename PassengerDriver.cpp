#include "PassengerDriver.h"
#include "Config.h"
#include "GameSymbols.h"
#include "Input.h"
#include "Log.h"
#include "Notifications.h"

static void* g_lastPed = nullptr;
static void* g_lastVehicle = nullptr;
static void* g_lastCandidateVehicle = nullptr;
static int g_loopCount = 0;

static const char* BoolText(bool value)
{
    return value ? "1" : "0";
}

static void SetEnabled(bool enabled)
{
    LPDSettings::SaveEnabled(enabled);

    if(enabled)
    {
        LPD_Log("[MODE] LuJim Passenger Driver ATIVADO");
        Notifications::Show("LuJim Passenger Driver: ATIVADO");
    }
    else
    {
        LPD_Log("[MODE] LuJim Passenger Driver DESATIVADO");
        Notifications::Show("LuJim Passenger Driver: DESATIVADO");
    }
}

void PassengerDriver::Init()
{
    LPD_Log(
        "[INIT] LuJim Passenger Driver iniciado. Enabled=%d HoldTime=%d ExperimentalHooks=%d",
        LPDSettings::values.enabled ? 1 : 0,
        LPDSettings::values.holdTimeMs,
        LPDSettings::values.experimentalHooks ? 1 : 0
    );

    LPD_Log(
        "[INIT] Modo salvo no INI: %s.",
        LPDSettings::values.enabled ? "ativado" : "desativado"
    );

    LPD_Log("[INIT] V3.0: teste de ativacao por botao. Nao move jogador e nao controla recruta.");
}

void PassengerDriver::Update(float dtMs)
{
    Input::Update(dtMs);

    if(Input::ConsumeToggleRequest())
    {
        SetEnabled(!LPDSettings::values.enabled);
    }

    void* ped = nullptr;
    void* vehicle = nullptr;
    void* candidateVehicle = nullptr;
    bool isDriver = false;

    FindPlayerPedFn findPed = GetFindPlayerPed();
    FindPlayerVehicleFn findVehicle = GetFindPlayerVehicle();
    VehicleIsDriverFn vehicleIsDriver = GetVehicleIsDriver();

    if(findPed)
    {
        ped = findPed(0);
    }

    if(findVehicle)
    {
        vehicle = findVehicle(0, false);
        candidateVehicle = findVehicle(0, true);
    }

    if(ped != g_lastPed)
    {
        if(ped)
            LPD_Log("[PLAYER] CJ encontrado. Ped=%p", ped);
        else
            LPD_Log("[PLAYER] CJ perdido/indisponivel");

        g_lastPed = ped;
    }

    if(candidateVehicle != g_lastCandidateVehicle)
    {
        LPD_Log(
            "[TARGET] V3.0 CandidateVehicle mudou. CandidateVehicle=%p CurrentVehicle=%p",
            candidateVehicle,
            vehicle
        );

        g_lastCandidateVehicle = candidateVehicle;
    }

    if(vehicle && ped && vehicleIsDriver)
    {
        isDriver = vehicleIsDriver(vehicle, ped);
    }

    if(vehicle != g_lastVehicle)
    {
        if(vehicle)
        {
            LPD_Log(
                "[PLAYER] Dentro de veiculo. Vehicle=%p Seat=%s",
                vehicle,
                isDriver ? "Motorista" : "Passageiro"
            );
        }
        else
        {
            LPD_Log("[PLAYER] Fora do veiculo");
        }

        g_lastVehicle = vehicle;
    }

    g_loopCount++;

    if(g_loopCount >= 4)
    {
        g_loopCount = 0;

        LPD_Log(
            "[MONITOR] V3.0 ativo. Ped=%p Vehicle=%p Candidate=%p Seat=%s Enabled=%s ExperimentalHooks=%s",
            ped,
            vehicle,
            candidateVehicle,
            vehicle ? (isDriver ? "Motorista" : "Passageiro") : "Fora do veiculo",
            BoolText(LPDSettings::values.enabled),
            BoolText(LPDSettings::values.experimentalHooks)
        );
    }
}

void PassengerDriver::Toggle()
{
    SetEnabled(!LPDSettings::values.enabled);
}
