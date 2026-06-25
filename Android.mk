LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := LuJimPassengerDriverAML
LOCAL_SRC_FILES := main.cpp
LOCAL_LDLIBS := -llog
LOCAL_CPPFLAGS := -std=c++17 -Wall -Wextra -Wno-unused-parameter
include $(BUILD_SHARED_LIBRARY)
