LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := LuJimPassengerDriver
LOCAL_SRC_FILES := main.cpp Config.cpp Log.cpp Notifications.cpp Input.cpp GameSymbols.cpp Recruit.cpp Vehicle.cpp PassengerDriver.cpp
LOCAL_CPPFLAGS := -std=c++17 -fexceptions -frtti -Wall -Wextra -DLPD_EXPERIMENTAL_V1
LOCAL_LDLIBS := -llog -landroid -ldl
include $(BUILD_SHARED_LIBRARY)
