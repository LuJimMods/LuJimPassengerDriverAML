#include "Input.h"
#include "Log.h"

bool Input::toggleRequest = false;
bool Input::lockedUntilRelease = false;
bool Input::fakePressed = false;
float Input::holdMs = 0.0f;
bool Input::lastPressed = false;

static constexpr float HOLD_REQUIRED_MS = 3000.0f;

static bool ReadEnterExitButton()
{
    /*
        V3.0:
        Por enquanto mantemos leitura segura.
        A leitura real do botão do GTA será ligada depois que confirmarmos
        o endereço correto do controle mobile.

        fakePressed existe para teste interno/debug.
    */
    return false;
}

void Input::Update(float dtMs)
{
    bool pressed = fakePressed || ReadEnterExitButton();

    if (pressed && !lastPressed)
    {
        LPD_Log("[INPUT] Botao Entrar/Sair pressionado");
    }

    if (!pressed && lastPressed)
    {
        LPD_Log("[INPUT] Botao Entrar/Sair solto");
        holdMs = 0.0f;
        lockedUntilRelease = false;
    }

    if (pressed && !lockedUntilRelease)
    {
        holdMs += dtMs;

        if (holdMs >= 1000.0f && holdMs - dtMs < 1000.0f)
            LPD_Log("[INPUT] Segurando botao: 1000ms");

        if (holdMs >= 2000.0f && holdMs - dtMs < 2000.0f)
            LPD_Log("[INPUT] Segurando botao: 2000ms");

        if (holdMs >= HOLD_REQUIRED_MS)
        {
            toggleRequest = true;
            lockedUntilRelease = true;
            LPD_Log("[INPUT] Segurando botao: 3000ms - alternar modo solicitado");
        }
    }

    lastPressed = pressed;
}

bool Input::ConsumeToggleRequest()
{
    if (!toggleRequest)
        return false;

    toggleRequest = false;
    return true;
}

void Input::DebugSetPressed(bool pressed)
{
    fakePressed = pressed;
}

float Input::HoldPercent()
{
    float percent = holdMs / HOLD_REQUIRED_MS;

    if (percent < 0.0f)
        percent = 0.0f;

    if (percent > 1.0f)
        percent = 1.0f;

    return percent;
}

bool Input::IsPressedNow()
{
    return fakePressed || ReadEnterExitButton();
}
