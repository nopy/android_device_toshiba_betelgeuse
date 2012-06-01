# Camera Setup
USE_CAMERA_STUB := false

# inherit from the proprietary version
-include vendor/toshiba/betelgeuse/BoardConfigVendor.mk

TARGET_BOARD_PLATFORM := tegra
TARGET_BOARD_INFO_FILE := device/toshiba/betelgeuse/board-info.txt
TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
TARGET_ARCH_VARIANT := armv7-a
TARGET_ARCH_VARIANT_CPU := cortex-a9
TARGET_ARCH_VARIANT_FPU := vfpv3-d16
TARGET_CPU_SMP := true

TARGET_NO_BOOTLOADER := true
TARGET_BOOTLOADER_BOARD_NAME := betelgeuse

ARCH_ARM_HAVE_TLS_REGISTER := true

#BOARD_USES_HGL := true
#BOARD_USES_OVERLAY := true
USE_OPENGL_RENDERER := true

# Modem
TARGET_NO_RADIOIMAGE := true

# Use Old Style USB Mounting Untill we get kernel source
BOARD_USE_USB_MASS_STORAGE_SWITCH := true

# custom recovery ui
BOARD_CUSTOM_RECOVERY_KEYMAPPING := ../../device/toshiba/betelgeuse/recovery/recovery_ui.c

# Wifi related defines
#CONFIG_DRIVER_AR6000 := true
#BOARD_WPA_SUPPLICANT_DRIVER := AR6000
#WPA_SUPPLICANT_VERSION      := VER_0_6_X
WIFI_DRIVER_MODULE_NAME     := ar6000
BOARD_WEXT_NO_COMBO_SCAN	:= true

BOARD_WPA_SUPPLICANT_DRIVER := WEXT
WPA_SUPPLICANT_VERSION := VER_0_8_X
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_ath6kl
BOARD_HOSTAPD_DRIVER := WEXT
BOARD_HOSTAPD_PRIVATE_LIB := lib_driver_cmd_ath6kl
BOARD_WLAN_DEVICE := ath6kl
BOARD_WLAN_ATHEROS_SDK := hardware/AR6kSDK.3.1/AR6kSDK.build_3.1_RC.667
BOARD_WLAN_CHIP_AR6003 := true

BOARD_HAVE_BLUETOOTH := true
BOARD_HAVE_BLUETOOTH_BCM := true
#BOARD_HAVE_BLUETOOTH_CSR := true

BOARD_KERNEL_CMDLINE := no_console_suspend=1 console=tty0,115200n8 video=tegrafb androidboot.hardware=betelgeuse

BOARD_KERNEL_BASE := 0x10000000
BOARD_PAGE_SIZE := 0x00000800

BOARD_USES_GENERIC_AUDIO := true
BOARD_USES_AUDIO_LEGACY := false
TARGET_USES_OLD_LIBSENSORS_HAL := false

BOARD_EGL_CFG := device/toshiba/betelgeuse/egl.cfg
TARGET_OTA_ASSERT_DEVICE := betelgeuse,folio100,GT-P7510

TARGET_USERIMAGES_USE_EXT4 := true
BOARD_BOOTIMAGE_PARTITION_SIZE := 8388608
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 5242880
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 268435456
BOARD_USERDATAIMAGE_PARTITION_SIZE := 1073741824
BOARD_FLASH_BLOCK_SIZE := 131072

# Setting this to avoid boot locks on the system from using the "misc" partition.
BOARD_HAS_NO_MISC_PARTITION := true

# Indicate that the board has an Internal SD Card
#BOARD_HAS_SDCARD_INTERNAL := true

BOARD_DATA_DEVICE := /dev/block/mmcblk0p5
BOARD_DATA_FILESYSTEM := ext4
BOARD_CACHE_DEVICE := /dev/block/mmcblk0p2
BOARD_CACHE_FILESYSTEM := ext4
BOARD_HAS_NO_SELECT_BUTTON := true

BOARD_VOLD_MAX_PARTITIONS := 11

BOARD_NO_ALLOW_DEQUEUE_CURRENT_BUFFER := true

# Use nicer font rendering
BOARD_USE_SKIA_LCDTEXT := true
