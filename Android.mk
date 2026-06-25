LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := LuJimPassengerDriver
LOCAL_SRC_FILES := main.cpp Config.cpp Input.cpp PassengerDriver.cpp Recruit.cpp Vehicle.cpp UI.cpp
LOCAL_LDLIBS := -llog
LOCAL_CPPFLAGS := -DANDROID -DLUJIM_PASSENGER_DRIVER
include $(BUILD_SHARED_LIBRARY)
