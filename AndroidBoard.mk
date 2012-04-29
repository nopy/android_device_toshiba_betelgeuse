LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

ALL_PREBUILT += $(INSTALLED_KERNEL_TARGET)
TARGET_KERNEL_SOURCE := kernel/toshiba/betelgeuse
TARGET_KERNEL_CONFIG := tegra_betelgeuse_android_defconfig

# include the non-open-source counterpart to this file
-include vendor/toshiba/betelgeuse/AndroidBoardVendor.mk
