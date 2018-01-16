LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := utils
LOCAL_SRC_FILES := Lock.cpp \
    SingleThread.cpp \
    Semaphore.cpp \
    MessageQueue.cpp

LOCAL_SHARED_LIBRARIES := libstlport

APP_ALLOW_MISSING_DEPS := true


include $(BUILD_STATIC_LIBRARY)
include $(call all-makefiles-under,$(LOCAL_PATH))