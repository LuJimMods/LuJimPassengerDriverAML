#include "Input.h"
#include "Config.h"
#include "Log.h"
bool Input::toggleRequest=false; bool Input::lockedUntilRelease=false; bool Input::fakePressed=false; float Input::holdMs=0.0f;
static bool IsEnterVehicleButtonPressed(){ return Input::ConsumeToggleRequest(), false; /* placeholder seguro: hook real do widget/CPad será ligado após teste */ }
void Input::DebugSetPressed(bool p){ fakePressed=p; }
void Input::Update(float dtMs){ bool pressed=fakePressed || IsEnterVehicleButtonPressed(); if(!pressed){ holdMs=0; lockedUntilRelease=false; return;} if(lockedUntilRelease) return; holdMs+=dtMs; if(holdMs>=Config::values.holdTimeMs){ toggleRequest=true; lockedUntilRelease=true; holdMs=0; LPD_Log("[INPUT] Hold completo: toggle solicitado"); }}
bool Input::ConsumeToggleRequest(){ if(!toggleRequest) return false; toggleRequest=false; return true; }
