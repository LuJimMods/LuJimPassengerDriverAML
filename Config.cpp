#include "Config.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>

static const char* kConfigDir = "/storage/emulated/0/Android_unprotected/data/com.rockstargames.gtasa/mods/configs";
static const char* kConfigPath = "/storage/emulated/0/Android_unprotected/data/com.rockstargames.gtasa/mods/configs/LuJimPassengerDriver.ini";

static int readInt(const char* line, const char* key, int def) {
    size_t n = strlen(key);
    if(strncmp(line, key, n) == 0 && line[n] == '=') return atoi(line + n + 1);
    return def;
}

namespace Config {
const char* Path() { return kConfigPath; }

void EnsureDefault() {
    mkdir(kConfigDir, 0777);
    if(access(kConfigPath, F_OK) == 0) return;
    ModConfig cfg;
    Save(cfg);
}

ModConfig Load() {
    EnsureDefault();
    ModConfig cfg;
    FILE* f = fopen(kConfigPath, "r");
    if(!f) return cfg;
    char line[256];
    while(fgets(line, sizeof(line), f)) {
        if(strncmp(line,"Enabled=",8)==0) cfg.enabled = readInt(line,"Enabled", cfg.enabled) != 0;
        else if(strncmp(line,"HoldTime=",9)==0) cfg.holdTimeMs = readInt(line,"HoldTime", cfg.holdTimeMs);
        else if(strncmp(line,"RequireRecruit=",15)==0) cfg.requireRecruit = readInt(line,"RequireRecruit", cfg.requireRecruit) != 0;
        else if(strncmp(line,"UseFirstRecruit=",16)==0) cfg.useFirstRecruit = readInt(line,"UseFirstRecruit", cfg.useFirstRecruit) != 0;
        else if(strncmp(line,"UseNearestRecruit=",18)==0) cfg.useNearestRecruit = readInt(line,"UseNearestRecruit", cfg.useNearestRecruit) != 0;
        else if(strncmp(line,"PatrolMode=",11)==0) cfg.patrolMode = readInt(line,"PatrolMode", cfg.patrolMode) != 0;
        else if(strncmp(line,"RealisticEntry=",15)==0) cfg.realisticEntry = readInt(line,"RealisticEntry", cfg.realisticEntry) != 0;
        else if(strncmp(line,"ShowProgressBar=",16)==0) cfg.showProgressBar = readInt(line,"ShowProgressBar", cfg.showProgressBar) != 0;
        else if(strncmp(line,"ShowNotifications=",18)==0) cfg.showNotifications = readInt(line,"ShowNotifications", cfg.showNotifications) != 0;
        else if(strncmp(line,"EnableLogs=",11)==0) cfg.enableLogs = readInt(line,"EnableLogs", cfg.enableLogs) != 0;
        else if(strncmp(line,"WaitOutsideVehicle=",19)==0) cfg.waitOutsideVehicle = readInt(line,"WaitOutsideVehicle", cfg.waitOutsideVehicle) != 0;
        else if(strncmp(line,"Cars=",5)==0) cfg.allowCars = readInt(line,"Cars", cfg.allowCars) != 0;
        else if(strncmp(line,"Police=",7)==0) cfg.allowPolice = readInt(line,"Police", cfg.allowPolice) != 0;
        else if(strncmp(line,"Bikes=",6)==0) cfg.allowBikes = readInt(line,"Bikes", cfg.allowBikes) != 0;
        else if(strncmp(line,"Boats=",6)==0) cfg.allowBoats = readInt(line,"Boats", cfg.allowBoats) != 0;
        else if(strncmp(line,"Planes=",7)==0) cfg.allowPlanes = readInt(line,"Planes", cfg.allowPlanes) != 0;
        else if(strncmp(line,"Helicopters=",12)==0) cfg.allowHelicopters = readInt(line,"Helicopters", cfg.allowHelicopters) != 0;
        else if(strncmp(line,"Custom19001_19200=",18)==0) cfg.allowCustom19001_19200 = readInt(line,"Custom19001_19200", cfg.allowCustom19001_19200) != 0;
    }
    fclose(f);
    if(cfg.holdTimeMs < 500) cfg.holdTimeMs = 3000;
    return cfg;
}

void Save(const ModConfig& cfg) {
    mkdir(kConfigDir, 0777);
    FILE* f = fopen(kConfigPath, "w");
    if(!f) return;
    fprintf(f,
        "[LuJimPassengerDriver]\n"
        "Enabled=%d\nHoldTime=%d\nRequireRecruit=%d\nUseFirstRecruit=%d\nUseNearestRecruit=%d\n"
        "PatrolMode=%d\nRealisticEntry=%d\nShowProgressBar=%d\nShowNotifications=%d\nEnableLogs=%d\nWaitOutsideVehicle=%d\n\n"
        "[AllowedVehicles]\nCars=%d\nPolice=%d\nBikes=%d\nBoats=%d\nPlanes=%d\nHelicopters=%d\nCustom19001_19200=%d\n",
        cfg.enabled, cfg.holdTimeMs, cfg.requireRecruit, cfg.useFirstRecruit, cfg.useNearestRecruit,
        cfg.patrolMode, cfg.realisticEntry, cfg.showProgressBar, cfg.showNotifications, cfg.enableLogs, cfg.waitOutsideVehicle,
        cfg.allowCars, cfg.allowPolice, cfg.allowBikes, cfg.allowBoats, cfg.allowPlanes, cfg.allowHelicopters, cfg.allowCustom19001_19200);
    fclose(f);
}

void SetEnabled(bool value) {
    ModConfig cfg = Load();
    cfg.enabled = value;
    Save(cfg);
}
}
