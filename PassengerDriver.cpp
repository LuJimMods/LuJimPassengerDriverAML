#include "PassengerDriver.h"
#include "Config.h"
#include "Input.h"
#include "Notifications.h"
#include "Log.h"
#include "Recruit.h"
#include "Vehicle.h"
void PassengerDriver::Init(){
    LPDSettings::Load();
    LPD_Log("[INIT] LuJim Passenger Driver iniciado. Enabled=%d HoldTime=%d ExperimentalHooks=%d", LPDSettings::values.enabled, LPDSettings::values.holdTimeMs, LPDSettings::values.experimentalHooks);
    if(LPDSettings::values.enabled) LPD_Log("[INIT] Modo salvo no INI: ativado."); else LPD_Log("[INIT] Modo salvo no INI: desativado.");
}
void PassengerDriver::Toggle(){
    LPDSettings::SaveEnabled(!LPDSettings::values.enabled);
    if(LPDSettings::values.enabled) Notifications::Enabled(); else Notifications::Disabled();
    LPD_Log("[STATE] Enabled=%d", LPDSettings::values.enabled);
}
void PassengerDriver::Update(float dtMs){
    Input::Update(dtMs);
    if(Input::ConsumeToggleRequest()) Toggle();
    if(!LPDSettings::values.enabled) return;
    if(!LPDSettings::values.experimentalHooks){
        static bool warned=false;
        if(!warned){ LPD_Log("[SAFE] ExperimentalHooks=0. Sistema de entrada/recruta/controlador está protegido contra crash."); warned=true; }
        return;
    }
    void* veh = Vehicle::FindTargetVehicle();
    void* rec = Recruit::FindValidRecruit();
    if(!veh || !rec){ Notifications::NoRecruit(); return; }
}
