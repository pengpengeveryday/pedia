LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := util_test
LOCAL_SRC_FILES := main.cpp

LOCAL_C_INCLUDES:= $(LOCAL_PATH)/../

LOCAL_SHARED_LIBRARIES :=

LOCAL_STATIC_LIBRARIES := libutils

include $(BUILD_EXECUTABLE)
