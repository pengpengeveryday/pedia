LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := \
    $(JNI_DIR)/core/ \
    $(JNI_DIR)/media/ \

LOCAL_SRC_FILES:= \
    JpegEncoder.cpp \
    HuffmanMaker.cpp \

LOCAL_STATIC_LIBRARIES := \
    datasource \

LOCAL_MODULE:= jpeg

include $(BUILD_EXECUTABLE)