#pragma once
#include <android/log.h>
#define LPD_TAG "LuJimPassengerDriver"
#define LPD_LOGI(...) __android_log_print(ANDROID_LOG_INFO, LPD_TAG, __VA_ARGS__)
#define LPD_LOGW(...) __android_log_print(ANDROID_LOG_WARN, LPD_TAG, __VA_ARGS__)
#define LPD_LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LPD_TAG, __VA_ARGS__)
