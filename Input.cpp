#include "Input.h"

// TODO: ligar nos endereços reais do botão ENTER/VEHICLE do GTA SA Android 2.00.
// Enquanto isso, esta base compila e mantém a lógica pronta.
static bool gPressed = false;
static bool gToggleRequest = false;
static bool gWasPressed = false;
static float gHold = 0.0f;
static int gHoldTime = 3000;

extern int LPD_GetHoldTimeMs();

namespace Input {
void Update(float dtMs) {
    gHoldTime = LPD_GetHoldTimeMs();
    bool pressed = IsEnterPressed();
    if(pressed) {
        gHold += dtMs;
        if(!gWasPressed && gHold >= gHoldTime) {
            gToggleRequest = true;
            gWasPressed = true;
        }
        if(gHold >= gHoldTime && !gWasPressed) {
            gToggleRequest = true;
            gWasPressed = true;
        }
    } else {
        gHold = 0.0f;
        gWasPressed = false;
    }
}

bool ConsumeToggleRequest() {
    if(!gToggleRequest) return false;
    gToggleRequest = false;
    return true;
}

float HoldProgress() {
    if(gHoldTime <= 0) return 0.0f;
    float p = gHold / (float)gHoldTime;
    if(p < 0.0f) p = 0.0f;
    if(p > 1.0f) p = 1.0f;
    return p;
}

bool IsEnterPressed() {
    return gPressed;
}
}
