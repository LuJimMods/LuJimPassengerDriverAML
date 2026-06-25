#include "Log.h"
#include "Config.h"
#include <android/log.h>
#include <cstdarg>
#include <cstdio>
#include <fstream>
#include <sys/stat.h>
#define LOG_TAG "LuJimPassengerDriver"
void LPD_Log(const char* fmt, ...) {
    char buf[1024]; va_list ap; va_start(ap, fmt); vsnprintf(buf,sizeof(buf),fmt,ap); va_end(ap);
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%s", buf);
    if(!Config::values.enableLogs) return;
    mkdir("/storage/emulated/0/Android_unprotected/data/com.rockstargames.gtasa/mods/logs",0777);
    std::ofstream f("/storage/emulated/0/Android_unprotected/data/com.rockstargames.gtasa/mods/logs/LuJimPassengerDriver.log", std::ios::app);
    if(f.good()) f << buf << "\n";
}
