LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := \
    $(JNI_DIR)/core/ \

LOCAL_SRC_FILES:= \
    test.cpp \
    BmpExtractor.cpp \

LOCAL_STATIC_LIBRARIES := \
    datasource \

LOCAL_MODULE:= bmp

include $(BUILD_EXECUTABLE)
include $(call all-makefiles-under,$(LOCAL_PATH))