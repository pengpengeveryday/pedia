LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= test.cpp \

LOCAL_MODULE:= pedia-jni

include $(BUILD_SHARED_LIBRARY)