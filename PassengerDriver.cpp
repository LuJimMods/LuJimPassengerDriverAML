#include "PassengerDriver.h"
#include "Config.h"
#include "Input.h"
#include "Notifications.h"
#include "Log.h"
#include "Recruit.h"
#include "Vehicle.h"
void PassengerDriver::Init(){
    Config::Load();
    LPD_Log("[INIT] LuJim Passenger Driver iniciado. Enabled=%d HoldTime=%d ExperimentalHooks=%d", Config::values.enabled, Config::values.holdTimeMs, Config::values.experimentalHooks);
    if(Config::values.enabled) LPD_Log("[INIT] Modo salvo no INI: ativado."); else LPD_Log("[INIT] Modo salvo no INI: desativado.");
}
void PassengerDriver::Toggle(){
    Config::SaveEnabled(!Config::values.enabled);
    if(Config::values.enabled) Notifications::Enabled(); else Notifications::Disabled();
    LPD_Log("[STATE] Enabled=%d", Config::values.enabled);
}
void PassengerDriver::Update(float dtMs){
    Input::Update(dtMs);
    if(Input::ConsumeToggleRequest()) Toggle();
    if(!Config::values.enabled) return;
    if(!Config::values.experimentalHooks){
        static bool warned=false;
        if(!warned){ LPD_Log("[SAFE] ExperimentalHooks=0. Sistema de entrada/recruta/controlador está protegido contra crash."); warned=true; }
        return;
    }
    void* veh = Vehicle::FindTargetVehicle();
    void* rec = Recruit::FindValidRecruit();
    if(!veh || !rec){ Notifications::NoRecruit(); return; }
}
