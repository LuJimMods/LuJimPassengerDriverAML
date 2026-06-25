#pragma once
class Input {
public:
    static void Update(float dtMs);
    static bool ConsumeToggleRequest();
    static void DebugSetPressed(bool p);
    static float HoldPercent();
private:
    static bool toggleRequest;
    static bool lockedUntilRelease;
    static bool fakePressed;
    static float holdMs;
};
