LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

JNI_DIR := $(LOCAL_PATH)

LOCAL_SRC_FILES:= test.cpp \

LOCAL_MODULE:= pedia-jni

include $(BUILD_SHARED_LIBRARY)
include $(call all-makefiles-under,$(LOCAL_PATH))