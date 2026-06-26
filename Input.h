#pragma once

class Input
{
public:
    static void Update(float dtMs);
    static bool ConsumeToggleRequest();
    static void DebugSetPressed(bool pressed);
    static float HoldPercent();
    static bool IsPressedNow();

private:
    static bool toggleRequest;
    static bool lockedUntilRelease;
    static bool fakePressed;
    static float holdMs;
    static bool lastPressed;
};
