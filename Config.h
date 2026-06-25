#pragma once
#include <string>

struct ModConfig {
    bool enabled = false;
    int holdTimeMs = 3000;
    bool requireRecruit = true;
    bool useFirstRecruit = true;
    bool useNearestRecruit = false;
    bool patrolMode = true;
    bool realisticEntry = true;
    bool showProgressBar = true;
    bool showNotifications = true;
    bool enableLogs = true;
    bool waitOutsideVehicle = true;

    bool allowCars = true;
    bool allowPolice = true;
    bool allowBikes = false;
    bool allowBoats = false;
    bool allowPlanes = false;
    bool allowHelicopters = false;
    bool allowCustom19001_19200 = true;
};

namespace Config {
    const char* Path();
    void EnsureDefault();
    ModConfig Load();
    void Save(const ModConfig& cfg);
    void SetEnabled(bool value);
}
