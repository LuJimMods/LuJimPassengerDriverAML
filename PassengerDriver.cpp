#include "PassengerDriver.h"
#include "Config.h"
#include "GameSymbols.h"
#include "Input.h"
#include "Log.h"
#include "Notifications.h"

#include <pthread.h>
#include <unistd.h>

static bool g_running = false;
static bool g_threadStarted = false;
static bool g_allModsLoaded = false;
static pthread_t g_thread;

static void SetEnabled(bool enabled)
{
    LPDSettings::enabled = enabled;
    LPDSettings::Save();

    if (enabled)
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

static const char* BoolText(bool value)
{
    return value ? "1" : "0";
}

static void* MonitorThread(void*)
{
    LPD_Log("[THREAD] Monitor seguro V3.0 iniciado");

    void* lastPed = nullptr;
    void* lastVehicle = nullptr;
    void* lastCandidateVehicle = nullptr;

    int loopCount = 0;

    while (g_running)
    {
        usleep(250000);

        Input::Update(250.0f);

        if (Input::ConsumeToggleRequest())
        {
            SetEnabled(!LPDSettings::enabled);
        }

        void* ped = nullptr;
        void* vehicle = nullptr;
        void* candidateVehicle = nullptr;
        bool isDriver = false;

        if (GameSymbols::FindPlayerPed)
        {
            ped = GameSymbols::FindPlayerPed(0);
        }

        if (GameSymbols::FindPlayerVehicle)
        {
            vehicle = GameSymbols::FindPlayerVehicle(0, false);
            candidateVehicle = GameSymbols::FindPlayerVehicle(0, true);
        }

        if (ped != lastPed)
        {
            if (ped)
                LPD_Log("[PLAYER] CJ encontrado. Ped=%p", ped);
            else
                LPD_Log("[PLAYER] CJ perdido/indisponivel");

            lastPed = ped;
        }

        if (candidateVehicle != lastCandidateVehicle)
        {
            LPD_Log(
                "[TARGET] V3.0 CandidateVehicle mudou. CandidateVehicle=%p CurrentVehicle=%p",
                candidateVehicle,
                vehicle
            );

            lastCandidateVehicle = candidateVehicle;
        }

        if (vehicle && ped && GameSymbols::CVehicle_IsDriver)
        {
            isDriver = GameSymbols::CVehicle_IsDriver(vehicle, ped);
        }

        if (vehicle != lastVehicle)
        {
            if (vehicle)
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

            lastVehicle = vehicle;
        }

        loopCount++;

        if (loopCount >= 4)
        {
            loopCount = 0;

            LPD_Log(
                "[MONITOR] V3.0 ativo. Ped=%p Vehicle=%p Candidate=%p Seat=%s Enabled=%s ExperimentalHooks=%s AllModsLoaded=%s",
                ped,
                vehicle,
                candidateVehicle,
                vehicle ? (isDriver ? "Motorista" : "Passageiro") : "Fora do veiculo",
                BoolText(LPDSettings::enabled),
                BoolText(LPDSettings::experimentalHooks),
                BoolText(g_allModsLoaded)
            );
        }
    }

    LPD_Log("[THREAD] Monitor seguro V3.0 finalizado");
    return nullptr;
}

void PassengerDriver::Init()
{
    if (g_threadStarted)
    {
        LPD_Log("[INIT] PassengerDriver::Init ignorado: thread ja iniciada");
        return;
    }

    LPD_Log(
        "[INIT] LuJim Passenger Driver iniciado. Enabled=%d HoldTime=%d ExperimentalHooks=%d",
        LPDSettings::enabled ? 1 : 0,
        LPDSettings::holdTimeMs,
        LPDSettings::experimentalHooks ? 1 : 0
    );

    LPD_Log(
        "[INIT] Modo salvo no INI: %s.",
        LPDSettings::enabled ? "ativado" : "desativado"
    );

    LPD_Log("[INIT] V3.0: teste de ativacao por botao. Nao move jogador e nao controla recruta.");

    g_running = true;

    if (pthread_create(&g_thread, nullptr, MonitorThread, nullptr) == 0)
    {
        g_threadStarted = true;
        LPD_Log("[THREAD] Monitor seguro criado com sucesso");
    }
    else
    {
        g_running = false;
        LPD_Log("[THREAD] Falha ao criar monitor seguro");
    }
}

void PassengerDriver::OnAllModsLoaded()
{
    g_allModsLoaded = true;
}

void PassengerDriver::Shutdown()
{
    if (!g_threadStarted)
        return;

    g_running = false;
    pthread_join(g_thread, nullptr);
    g_threadStarted = false;
}
