#pragma once
class Notifications {
public:
    static void Show(const char* text);
    static void Enabled();
    static void Disabled();
    static void NoRecruit();
    static void Progress(float percent, bool enabling);
};
