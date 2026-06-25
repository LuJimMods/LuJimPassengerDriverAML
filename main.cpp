#include "PassengerDriver.h"
#include "Log.h"
#include <cstdint>

extern "C" void OnModLoad() {
    LPD_LOGI("========================================");
    LPD_LOGI("LuJim Passenger Driver v1.0 by LuJim Mods");
    LPD_LOGI("GTA SA Android 2.00 AML - armeabi-v7a");
    LPD_LOGI("========================================");
    PassengerDriver::Init();
}

// Alguns loaders/projetos usam este nome alternativo.
extern "C" void aml_mod_init() {
    OnModLoad();
}
