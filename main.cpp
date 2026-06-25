#include "PassengerDriver.h"
#include "GameSymbols.h"
#include "Log.h"
#include <jni.h>
#include <pthread.h>
#include <unistd.h>
static bool gStarted = false;
static void* Loop(void*){
    sleep(2);
    while(true){
        PassengerDriver::Update(16.0f);
        usleep(16000);
    }
    return nullptr;
}
extern "C" void OnModLoad(){
    InitGameSymbols();
    PassengerDriver::Init();
    LPD_Log("[AML] OnModLoad concluido.");
    if(!gStarted){
        pthread_t t;
        pthread_create(&t, nullptr, Loop, nullptr);
        pthread_detach(t);
        gStarted = true;
    }
}
__attribute__((constructor)) static void lib_main(){
    // AML chamará OnModLoad. Constructor fica leve para evitar conflito no carregamento.
}
extern "C" jint JNI_OnLoad(JavaVM*, void*){ return JNI_VERSION_1_6; }
