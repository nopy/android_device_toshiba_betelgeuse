#
# Copyright (C) 2011 The Android Open-Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

ifndef BUILD_KERNEL
ifeq ($(TARGET_PREBUILT_KERNEL),)
LOCAL_KERNEL := device/toshiba/betelgeuse/kernel
else
LOCAL_KERNEL := $(TARGET_PREBUILT_KERNEL)
endif
PRODUCT_COPY_FILES := \
    $(LOCAL_KERNEL):kernel 
endif

PRODUCT_COPY_FILES += \
    device/toshiba/betelgeuse/init.betelgeuse.rc:root/init.betelgeuse.rc \
    device/toshiba/betelgeuse/init.betelgeuse.usb.rc:root/init.betelgeuse.usb.rc \
    device/toshiba/betelgeuse/ueventd.betelgeuse.rc:root/ueventd.betelgeuse.rc \
    device/toshiba/betelgeuse/media_profiles.xml:system/etc/media_profiles.xml \
    device/toshiba/betelgeuse/egalax_i2c.idc:system/usr/idc/egalax_i2c.idc \
    frameworks/base/data/etc/android.hardware.camera.flash-autofocus.xml:system/etc/permissions/android.hardware.camera.flash-autofocus.xml \
    frameworks/base/data/etc/android.hardware.camera.front.xml:system/etc/permissions/android.hardware.camera.front.xml \
    frameworks/base/data/etc/android.hardware.camera.xml:system/etc/permissions/android.hardware.camera.xml \
    frameworks/base/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    frameworks/base/data/etc/android.hardware.sensor.accelerometer.xml:system/etc/permissions/android.hardware.sensor.accelerometer.xml \
    frameworks/base/data/etc/android.hardware.sensor.compass.xml:system/etc/permissions/android.hardware.sensor.compass.xml \
    frameworks/base/data/etc/android.hardware.sensor.gyroscope.xml:system/etc/permissions/android.hardware.sensor.gyroscope.xml \
    frameworks/base/data/etc/android.hardware.sensor.light.xml:system/etc/permissions/android.hardware.sensor.light.xml \
    frameworks/base/data/etc/android.hardware.touchscreen.multitouch.jazzhand.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.jazzhand.xml \
    frameworks/base/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
    frameworks/base/data/etc/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml \
    frameworks/base/data/etc/android.hardware.usb.accessory.xml:system/etc/permissions/android.hardware.usb.accessory.xml \
    frameworks/base/data/etc/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml \
    frameworks/base/data/etc/tablet_core_hardware.xml:system/etc/permissions/tablet_core_hardware.xml 

# Vold
PRODUCT_COPY_FILES += \
        $(LOCAL_PATH)/vold.fstab:system/etc/vold.fstab


# Kernel modules
#PRODUCT_COPY_FILES += \
#    device/toshiba/betelgeuse/modules/scsi_wait_scan.ko:system/modules/scsi_wait_scan.ko \
#    device/toshiba/betelgeuse/modules/bcm4329.ko:system/modules/bcm4329.ko 

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/wifi/dhcpcd.conf:system/etc/dhcpcd/dhcpcd.conf \
    $(LOCAL_PATH)/wifi/wpa_supplicant.conf:system/etc/wifi/wpa_supplicant.conf \
    $(LOCAL_PATH)/wifi/ar3k/30101/PS_ASIC.pst:/system/wifi/ar3k/30101/PS_ASIC.pst \
    $(LOCAL_PATH)/wifi/ar3k/30101/RamPatch.txt:/system/wifi/ar3k/30101/RamPatch.txt \
    $(LOCAL_PATH)/wifi/ar3k/30101coex/PS_ASIC.pst:/system/wifi/ar3k/30101coex/PS_ASIC.pst \
    $(LOCAL_PATH)/wifi/ar3k/30101coex/RamPatch.txt:/system/wifi/ar3k/30101coex/RamPatch.txt \
    $(LOCAL_PATH)/wifi/ath6k/AR6003/hw2.0/otp.bin.z77:/system/wifi/ath6k/AR6003/hw2.0/otp.bin.z77 \
    $(LOCAL_PATH)/wifi/ath6k/AR6003/hw2.0/data.patch.bin:/system/wifi/ath6k/AR6003/hw2.0/data.patch.bin \
    $(LOCAL_PATH)/wifi/ath6k/AR6003/hw2.0/athwlan.bin.z77:/system/wifi/ath6k/AR6003/hw2.0/athwlan.bin.z77 \
    $(LOCAL_PATH)/wifi/ath6k/AR6003/hw2.0/athtcmd_ram.bin:/system/wifi/ath6k/AR6003/hw2.0/athtcmd_ram.bin \
    $(LOCAL_PATH)/wifi/ath6k/AR6003/hw2.0/bdata.SD31.bin:/system/wifi/ath6k/AR6003/hw2.0/bdata.SD31.bin \
    $(LOCAL_PATH)/wifi/ath6k/AR6003/hw2.0/bdata.WB31.bin:/system/wifi/ath6k/AR6003/hw2.0/bdata.WB31.bin \
    $(LOCAL_PATH)/wifi/libhuawei-ril.so:/system/lib/libhuawei-ril.so

PRODUCT_PACKAGES := \
    make_ext4fs \
    com.android.future.usb.accessory

PRODUCT_PROPERTY_OVERRIDES := \
    ro.opengles.version=131072 \
    wifi.interface=wlan0 \
    keyguard.no_require_sim=true \
    ro.telephony.ril.v3=icccardstatus,datacall,signalstrength \
    hwui.render_dirty_regions=false


PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
    ro.secure=0 \
    persist.sys.strictmode.visual=0

ADDITIONAL_DEFAULT_PROPERTIES += ro.secure=0
ADDITIONAL_DEFAULT_PROPERTIES += persist.sys.strictmode.visual=0

PRODUCT_CHARACTERISTICS := tablet

DEVICE_PACKAGE_OVERLAYS := \
    device/toshiba/betelgeuse/overlay

PRODUCT_TAGS += dalvik.gc.type-precise

PRODUCT_PACKAGES += \
    librs_jni \
    audio.primary.tegra \
    liba2dp \
    lights.tegra \
    com.android.future.usb.accessory \
    camera.tegra \
    libpkip \
    libaudioutils \
    tinyplay \
    tinycap \
    tinymix \
    hotplug \
    sensors.tegra

# Filesystem management tools
PRODUCT_PACKAGES += \
    make_ext4fs

$(call inherit-product-if-exists, vendor/toshiba/betelgeuse/device-vendor.mk)
$(call inherit-product, frameworks/base/build/phone-hdpi-512-dalvik-heap.mk)
$(call inherit-product-if-exists, hardware/AR6kSDK.3.0/host/device-ath6k.mk)

