#include "Input.h"
#include "Log.h"

#include <dlfcn.h>
#include <cstdio>
#include <cstring>
#include <cstdint>

bool Input::toggleRequest = false;
bool Input::lockedUntilRelease = false;
bool Input::fakePressed = false;
float Input::holdMs = 0.0f;
bool Input::lastPressed = false;

static constexpr float HOLD_REQUIRED_MS = 3000.0f;
static constexpr uint16_t KEY_ENTER_EXIT_VEHICLE = 15;

using GetPadStateFn = int (*)(uint16_t player, uint16_t key);

static GetPadStateFn gGetPadState = nullptr;
static bool gTriedResolve = false;

static uintptr_t FindLibGTASABase()
{
    FILE* fp = fopen("/proc/self/maps", "r");
    if (!fp) return 0;

    char line[512];
    while (fgets(line, sizeof(line), fp))
    {
        if (strstr(line, "libGTASA.so"))
        {
            uintptr_t start = 0;
            sscanf(line, "%x-", &start);
            fclose(fp);
            return start;
        }
    }

    fclose(fp);
    return 0;
}

static void ResolveGetPadState()
{
    if (gTriedResolve) return;
    gTriedResolve = true;

    void* handle = dlopen("libGTASA.so", RTLD_NOW);
    if (handle)
    {
        gGetPadState = (GetPadStateFn)dlsym(handle, "_ZN14CRunningScript11GetPadStateEtt");
        if (gGetPadState)
        {
            LPD_Log("[INPUT] CRunningScript::GetPadState resolvido via dlsym: %p", (void*)gGetPadState);
            return;
        }
    }

    uintptr_t base = FindLibGTASABase();
    if (base)
    {
        gGetPadState = (GetPadStateFn)(base + 0x34d78d);
        LPD_Log("[INPUT] CRunningScript::GetPadState resolvido via offset: %p base=0x%x", (void*)gGetPadState, (unsigned int)base);
    }
    else
    {
        LPD_Log("[INPUT] Falha ao resolver libGTASA base para GetPadState");
    }
}

static bool ReadEnterExitButton()
{
    ResolveGetPadState();

    if (!gGetPadState)
        return false;

    int state = gGetPadState(0, KEY_ENTER_EXIT_VEHICLE);
    return state != 0;
}

void Input::Update(float dtMs)
{
    bool pressed = fakePressed || ReadEnterExitButton();

    if (pressed && !lastPressed)
    {
        LPD_Log("[INPUT] Key15 Entrar/Sair pressionada");
    }

    if (!pressed && lastPressed)
    {
        LPD_Log("[INPUT] Key15 Entrar/Sair solta");
        holdMs = 0.0f;
        lockedUntilRelease = false;
    }

    if (pressed && !lockedUntilRelease)
    {
        holdMs += dtMs;

        if (holdMs >= 1000.0f && holdMs - dtMs < 1000.0f)
            LPD_Log("[INPUT] Segurando Key15: 1000ms");

        if (holdMs >= 2000.0f && holdMs - dtMs < 2000.0f)
            LPD_Log("[INPUT] Segurando Key15: 2000ms");

        if (holdMs >= HOLD_REQUIRED_MS)
        {
            toggleRequest = true;
            lockedUntilRelease = true;
            LPD_Log("[INPUT] Segurando Key15: 3000ms - alternar modo solicitado");
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

    if (percent < 0.0f) percent = 0.0f;
    if (percent > 1.0f) percent = 1.0f;

    return percent;
}

bool Input::IsPressedNow()
{
    return fakePressed || ReadEnterExitButton();
}
