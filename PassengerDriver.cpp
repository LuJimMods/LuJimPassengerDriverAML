#include "PassengerDriver.h"
#include "Config.h"
#include "Input.h"
#include "Notifications.h"
#include "Log.h"
void PassengerDriver::Init(){ Config::Load(); LPD_Log("[INIT] LuJim Passenger Driver iniciado. Enabled=%d HoldTime=%d", Config::values.enabled, Config::values.holdTimeMs); }
void PassengerDriver::Toggle(){ Config::SaveEnabled(!Config::values.enabled); if(Config::values.enabled) Notifications::Enabled(); else Notifications::Disabled(); LPD_Log("[STATE] Enabled=%d", Config::values.enabled); }
void PassengerDriver::Update(float dtMs){ Input::Update(dtMs); if(Input::ConsumeToggleRequest()) Toggle(); if(!Config::values.enabled) return; /* V1 real: sequência recruta + passageiro será ligada aqui */ }
