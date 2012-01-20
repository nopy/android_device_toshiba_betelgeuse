LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# Include kerne headers in a proper way later on
#LOCAL_C_INCLUDES:= $(LOCAL_PATH)/../../../../kernel/include

LOCAL_MODULE := sensors.$(TARGET_BOARD_PLATFORM)
LOCAL_MODULE_TAGS := optional

LOCAL_PRELINK_MODULE := false

LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw

LOCAL_SHARED_LIBRARIES := liblog libcutils
# include any shared library dependencies

LOCAL_SRC_FILES := sensors.c

include $(BUILD_SHARED_LIBRARY)
