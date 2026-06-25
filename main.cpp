#include "PassengerDriver.h"
#include "GameSymbols.h"
#include "Log.h"
#include <jni.h>
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>

// -----------------------------------------------------------------------------
// AML metadata sem depender do SDK AML.
// O AML procura o simbolo __GetModInfo(). Sem isso a .so pode compilar,
// mas nao aparecer na lista de mods carregados.
// -----------------------------------------------------------------------------
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

static void lpd_copy(char* dst, size_t size, const char* src) {
    if(!dst || size == 0) return;
    std::snprintf(dst, size, "%s", src ? src : "");
}

static ModInfoCompat gModInfo;
static bool gModInfoReady = false;
static bool gStarted = false;

static void PrepareModInfo() {
    if(gModInfoReady) return;
    lpd_copy(gModInfo.szGUID, sizeof(gModInfo.szGUID), "lujim.passengerdriver");
    lpd_copy(gModInfo.szName, sizeof(gModInfo.szName), "LuJim Passenger Driver");
    lpd_copy(gModInfo.szVersion, sizeof(gModInfo.szVersion), "1.0.0");
    lpd_copy(gModInfo.szAuthor, sizeof(gModInfo.szAuthor), "LuJim Mods");
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

static void* Loop(void*){
    sleep(2);
    while(true){
        PassengerDriver::Update(16.0f);
        usleep(16000);
    }
    return nullptr;
}

extern "C" __attribute__((visibility("default"))) void OnModPreLoad(){
    PrepareModInfo();
    LPD_Log("[AML] OnModPreLoad chamado. Metadata OK: LuJim Passenger Driver");
}

extern "C" __attribute__((visibility("default"))) void OnModLoad(){
    PrepareModInfo();
    InitGameSymbols();
    PassengerDriver::Init();
    LPD_Log("[AML] OnModLoad concluido. GUID=lujim.passengerdriver Nome=LuJim Passenger Driver");
    if(!gStarted){
        pthread_t t;
        pthread_create(&t, nullptr, Loop, nullptr);
        pthread_detach(t);
        gStarted = true;
    }
}

extern "C" __attribute__((visibility("default"))) void OnAllModsLoaded(){
    LPD_Log("[AML] OnAllModsLoaded chamado.");
}

__attribute__((constructor)) static void lib_main(){
    // Se a biblioteca for aberta com dlopen, este log confirma que ela entrou na memoria.
    // Mantido leve para evitar crash antes do AML inicializar tudo.
    PrepareModInfo();
}

extern "C" __attribute__((visibility("default"))) jint JNI_OnLoad(JavaVM*, void*){
    PrepareModInfo();
    LPD_Log("[JNI] JNI_OnLoad chamado.");
    return JNI_VERSION_1_6;
}
