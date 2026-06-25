#include "PassengerDriver.h"
#include "Config.h"
#include "Input.h"
#include "Notifications.h"
#include "Log.h"
#include "Recruit.h"
#include "Vehicle.h"

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

    LPD_Log("[INIT] V2.1.0: monitor seguro ativo, sem mover jogador e sem controlar recruta.");
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

    if(!LPDSettings::values.enabled)
    {
        return;
    }

    if(!LPDSettings::values.experimentalHooks)
    {
        static bool warned = false;
        if(!warned)
        {
            LPD_Log("[SAFE] ExperimentalHooks=0. Monitor seguro ligado, hooks reais bloqueados para evitar crash.");
            warned = true;
        }
        return;
    }

    // Etapa experimental futura: aqui vamos detectar veiculo/recruta.
    // Por enquanto, Vehicle::FindTargetVehicle e Recruit::FindValidRecruit retornam nullptr com log seguro.
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
