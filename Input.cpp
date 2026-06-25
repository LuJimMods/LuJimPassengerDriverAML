#include "Input.h"
#include "Config.h"
#include "Log.h"
#include "Notifications.h"
bool Input::toggleRequest=false;
bool Input::lockedUntilRelease=false;
bool Input::fakePressed=false;
float Input::holdMs=0.0f;
static bool IsEnterVehicleButtonPressed(){
    // Seguro: ainda sem hook no CPad/TouchInterface.
    // Retorna falso para evitar crash até confirmar o offset do botão Entrar na libGTASA do usuário.
    return false;
}
void Input::DebugSetPressed(bool p){ fakePressed=p; }
float Input::HoldPercent(){ return Config::values.holdTimeMs > 0 ? holdMs / (float)Config::values.holdTimeMs : 0.0f; }
void Input::Update(float dtMs){
    bool pressed = fakePressed || IsEnterVehicleButtonPressed();
    if(!pressed){ holdMs=0; lockedUntilRelease=false; return; }
    if(lockedUntilRelease) return;
    holdMs += dtMs;
    if(Config::values.showProgressBar) Notifications::Progress(HoldPercent(), !Config::values.enabled);
    if(holdMs >= Config::values.holdTimeMs){
        toggleRequest = true;
        lockedUntilRelease = true;
        holdMs = 0;
        LPD_Log("[INPUT] Hold completo: toggle solicitado");
    }
}
bool Input::ConsumeToggleRequest(){ if(!toggleRequest) return false; toggleRequest=false; return true; }
