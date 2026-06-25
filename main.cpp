#include "PassengerDriver.h"
#include "GameSymbols.h"
#include "Log.h"

#include <jni.h>
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>

// Metadata para o AML reconhecer o mod
struct ModVersionCompat {
    unsigned short major = 1;
    unsigned short minor = 0;
    unsigned short revision = 0;
    unsigned short build = 0;
};

struct ModInfoCompat {
    char szGUID[48];
    char szName[48];
    char szVersion[24];
    char szAuthor[48];
    ModVersionCompat version;
};

static ModInfoCompat gModInfo;
static bool gModInfoReady = false;
static bool gStarted = false;

static void LPD_Copy(char* dst, size_t size, const char* src) {
    if (!dst || size == 0) return;
    std::snprintf(dst, size, "%s", src ? src : "");
}

static void PrepareModInfo() {
    if (gModInfoReady) return;

    LPD_Copy(gModInfo.szGUID, sizeof(gModInfo.szGUID), "lujim.passengerdriver");
    LPD_Copy(gModInfo.szName, sizeof(gModInfo.szName), "LuJim Passenger Driver");
    LPD_Copy(gModInfo.szVersion, sizeof(gModInfo.szVersion), "1.0.0");
    LPD_Copy(gModInfo.szAuthor, sizeof(gModInfo.szAuthor), "LuJim Mods");

    gModInfo.version.major = 1;
    gModInfo.version.minor = 0;
    gModInfo.version.revision = 0;
    gModInfo.version.build = 0;

    gModInfoReady = true;
}

extern "C" __attribute__((visibility("default"))) void* __GetModInfo() {
    PrepareModInfo();
    return &gModInfo;
}

extern "C" __attribute__((visibility("default"))) const char* __INeedASpecificGame() {
    return "com.rockstargames.gtasa";
}

static void* LPD_Loop(void*) {
    sleep(2);

    while (true) {
        PassengerDriver::Update(16.0f);
        usleep(16000);
    }

    return nullptr;
}

extern "C" __attribute__((visibility("default"))) void OnModPreLoad() {
    PrepareModInfo();
    LPD_Log("[AML] OnModPreLoad chamado - LuJim Passenger Driver");
}

extern "C" __attribute__((visibility("default"))) void OnModLoad() {
    PrepareModInfo();

    LPD_Log("[AML] OnModLoad iniciado - LuJim Passenger Driver");

    InitGameSymbols();
    PassengerDriver::Init();

    if (!gStarted) {
        pthread_t thread;
        pthread_create(&thread, nullptr, LPD_Loop, nullptr);
        pthread_detach(thread);
        gStarted = true;
    }

    LPD_Log("[AML] OnModLoad concluido - LuJim Passenger Driver carregado");
}

extern "C" __attribute__((visibility("default"))) void OnAllModsLoaded() {
    LPD_Log("[AML] OnAllModsLoaded chamado - LuJim Passenger Driver");
}

__attribute__((constructor)) static void LPD_Constructor() {
    PrepareModInfo();
}

extern "C" __attribute__((visibility("default"))) jint JNI_OnLoad(JavaVM*, void*) {
    PrepareModInfo();
    LPD_Log("[JNI] JNI_OnLoad chamado - LuJim Passenger Driver");
    return JNI_VERSION_1_6;
}
