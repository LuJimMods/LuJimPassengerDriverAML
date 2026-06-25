#pragma once
struct LPDConfig {
    bool enabled = false;
    int holdTimeMs = 3000;
    bool showNotifications = true;
    bool showProgressBar = true;
    bool playSounds = true;
    int notificationTimeMs = 2500;
    bool requireRecruit = true;
    bool useFirstRecruit = true;
    bool useNearestRecruit = false;
    bool patrolMode = true;
    bool realisticEntry = true;
    bool enableLogs = true;
    bool experimentalHooks = false;
};
class LPDSettings {
public:
    static LPDConfig values;
    static const char* Path();
    static void EnsureDefault();
    static bool Load();
    static bool Save();
    static bool SaveEnabled(bool enabled);
};
