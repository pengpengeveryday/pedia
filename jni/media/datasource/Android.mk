LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := \
    $(JNI_DIR)/core/ \

LOCAL_SRC_FILES:= \
    DataSource.cpp \
    FileDataSource.cpp \


LOCAL_MODULE:= datasource

include $(BUILD_STATIC_LIBRARY)