LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_CPP_EXTENSION := .cpp .cc
LOCAL_CPP_FEATURES += exceptions

LOCAL_MODULE := LuJimPassengerDriver
LOCAL_SRC_FILES := \
    main.cpp \
    mod/logger.cpp \
    mod/config.cpp \
    Config.cpp \
    Log.cpp \
    Notifications.cpp \
    Input.cpp \
    GameSymbols.cpp \
    Recruit.cpp \
    Vehicle.cpp \
    PassengerDriver.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_CPPFLAGS := -std=c++17 -fexceptions -frtti -Wall -Wextra -DLPD_SAFE_TEST_BUILD
LOCAL_LDLIBS := -llog -landroid -ldl

include $(BUILD_SHARED_LIBRARY)
