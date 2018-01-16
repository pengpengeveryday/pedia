LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= Lock.cpp \
    MessageQueue.cpp \
    Semaphore.cpp \
    SingleThread.cpp \

LOCAL_MODULE:= core

include $(BUILD_SHARED_LIBRARY)