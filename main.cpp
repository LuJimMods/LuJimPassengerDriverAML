#include "PassengerDriver.h"
#include "GameSymbols.h"
#include "Log.h"
#include <jni.h>
#include <pthread.h>
#include <unistd.h>
extern "C" void OnModLoad(){ InitGameSymbols(); PassengerDriver::Init(); LPD_Log("[AML] OnModLoad concluído."); }
static void* Loop(void*){ while(true){ PassengerDriver::Update(16.0f); usleep(16000); } return nullptr; }
__attribute__((constructor)) static void lib_main(){ pthread_t t; pthread_create(&t,nullptr,Loop,nullptr); pthread_detach(t); }
extern "C" jint JNI_OnLoad(JavaVM*, void*){ return JNI_VERSION_1_6; }
