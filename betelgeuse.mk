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

PRODUCT_COPY_FILES += \
    device/toshiba/betelgeuse/init.betelgeuse.rc:root/init.betelgeuse.rc \
    device/toshiba/betelgeuse/init.nv_dev_board.usb.rc:root/init.nv_dev_board.usb.rc \
    device/toshiba/betelgeuse/ueventd.betelgeuse.rc:root/ueventd.betelgeuse.rc \
    device/toshiba/betelgeuse/egalax_ts.idc:system/usr/idc/egalax_ts.idc \
    frameworks/native/data/etc/android.hardware.camera.flash-autofocus.xml:system/etc/permissions/android.hardware.camera.flash-autofocus.xml \
    frameworks/native/data/etc/android.hardware.camera.front.xml:system/etc/permissions/android.hardware.camera.front.xml \
    frameworks/native/data/etc/android.hardware.camera.xml:system/etc/permissions/android.hardware.camera.xml \
    frameworks/native/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    frameworks/native/data/etc/android.hardware.sensor.accelerometer.xml:system/etc/permissions/android.hardware.sensor.accelerometer.xml \
    frameworks/native/data/etc/android.hardware.sensor.compass.xml:system/etc/permissions/android.hardware.sensor.compass.xml \
    frameworks/native/data/etc/android.hardware.sensor.gyroscope.xml:system/etc/permissions/android.hardware.sensor.gyroscope.xml \
    frameworks/native/data/etc/android.hardware.sensor.light.xml:system/etc/permissions/android.hardware.sensor.light.xml \
    frameworks/native/data/etc/android.hardware.touchscreen.multitouch.jazzhand.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.jazzhand.xml \
    frameworks/native/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
    frameworks/native/data/etc/android.hardware.wifi.direct.xml:system/etc/permissions/android.hardware.wifi.direct.xml \
    frameworks/native/data/etc/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml \
    frameworks/native/data/etc/android.hardware.usb.accessory.xml:system/etc/permissions/android.hardware.usb.accessory.xml \
    frameworks/native/data/etc/tablet_core_hardware.xml:system/etc/permissions/tablet_core_hardware.xml 

# Graphics
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/media_profiles.xml:system/etc/media_profiles.xml

# Codecs
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/media_codecs.xml:system/etc/media_codecs.xml

# Audio policy configuration
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/audio_policy.conf:system/etc/audio_policy.conf

# Keychars
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/keychars/cpcap-key.kcm:system/usr/keychars/cpcap-key.kcm \
    $(LOCAL_PATH)/keychars/gpio-keys.kcm:system/usr/keychars/gpio-keys.kcm \
    $(LOCAL_PATH)/keychars/nvec_cir.kcm:system/usr/keychars/nvec_cir.kcm \
    $(LOCAL_PATH)/keychars/nvec_keyboard.kcm:system/usr/keychars/nvec_keyboard.kcm \
    $(LOCAL_PATH)/keychars/tegra-kbc.kcm:system/usr/keychars/tegra-kbc.kcm

# Keylayout
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/keylayout/cpcap-key.kl:system/usr/keylayout/cpcap-key.kl \
    $(LOCAL_PATH)/keylayout/gpio-keys.kl:system/usr/keylayout/gpio-keys.kl \
    $(LOCAL_PATH)/keylayout/nvec_cir.kl:system/usr/keylayout/nvec_cir.kl \
    $(LOCAL_PATH)/keylayout/nvec_keyboard.kl:system/usr/keylayout/nvec_keyboard.kl \
    $(LOCAL_PATH)/keylayout/tegra-kbc.kl:system/usr/keylayout/tegra-kbc.kl

# Vold
PRODUCT_COPY_FILES += \
        $(LOCAL_PATH)/vold.fstab:system/etc/vold.fstab

# Bluetooth firmware
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/wifi/ar3k/30101coex/PS_ASIC.pst:/system/wifi/ar3k/30101coex/PS_ASIC.pst \
    $(LOCAL_PATH)/wifi/ar3k/30101coex/RamPatch.txt:/system/wifi/ar3k/30101coex/RamPatch.txt

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/wifi/wpa_supplicant.conf:system/etc/wifi/wpa_supplicant.conf

# Some files for 3G
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/ppp/ip-up:/system/etc/ppp/ip-up \
    $(LOCAL_PATH)/ppp/ip-down:/system/etc/ppp/ip-down

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/asound.conf:/system/etc/asound.conf \
    $(LOCAL_PATH)/libasound.so:/system/lib/libasound.so \
    $(LOCAL_PATH)/alsa/alsa.conf:/system/usr/share/alsa/alsa.conf \
    $(LOCAL_PATH)/alsa/cards/aliases.conf:/system/usr/share/alsa/cards/aliases.conf \
    $(LOCAL_PATH)/alsa/pcm/modem.conf:/system/usr/share/alsa/pcm/modem.conf \
    $(LOCAL_PATH)/alsa/pcm/iec958.conf:/system/usr/share/alsa/pcm/iec958.conf \
    $(LOCAL_PATH)/alsa/pcm/dpl.conf:/system/usr/share/alsa/pcm/dpl.conf \
    $(LOCAL_PATH)/alsa/pcm/surround50.conf:/system/usr/share/alsa/pcm/surround50.conf \
    $(LOCAL_PATH)/alsa/pcm/center_lfe.conf:/system/usr/share/alsa/pcm/center_lfe.conf \
    $(LOCAL_PATH)/alsa/pcm/surround51.conf:/system/usr/share/alsa/pcm/surround51.conf \
    $(LOCAL_PATH)/alsa/pcm/dsnoop.conf:/system/usr/share/alsa/pcm/dsnoop.conf \
    $(LOCAL_PATH)/alsa/pcm/side.conf:/system/usr/share/alsa/pcm/side.conf \
    $(LOCAL_PATH)/alsa/pcm/dmix.conf:/system/usr/share/alsa/pcm/dmix.conf \
    $(LOCAL_PATH)/alsa/pcm/default.conf:/system/usr/share/alsa/pcm/default.conf \
    $(LOCAL_PATH)/alsa/pcm/surround40.conf:/system/usr/share/alsa/pcm/surround40.conf \
    $(LOCAL_PATH)/alsa/pcm/surround41.conf:/system/usr/share/alsa/pcm/surround41.conf \
    $(LOCAL_PATH)/alsa/pcm/front.conf:/system/usr/share/alsa/pcm/front.conf \
    $(LOCAL_PATH)/alsa/pcm/rear.conf:/system/usr/share/alsa/pcm/rear.conf \
    $(LOCAL_PATH)/alsa/pcm/surround71.conf:/system/usr/share/alsa/pcm/surround71.conf

PRODUCT_PACKAGES := \
    make_ext4fs \
    com.android.future.usb.accessory \
    hwprops

PRODUCT_PROPERTY_OVERRIDES := \
    ro.opengles.version=131072 \
    wifi.interface=wlan0 \
    wifi.p2pinterface=wlan1 \
    keyguard.no_require_sim=true \
    hwui.render_dirty_regions=false \
    ro.sf.lcd_density=120


PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
    ro.secure=0 \
    persist.sys.strictmode.visual=0

ADDITIONAL_DEFAULT_PROPERTIES += ro.secure=0
ADDITIONAL_DEFAULT_PROPERTIES += persist.sys.strictmode.visual=0

PRODUCT_CHARACTERISTICS := tablet

PRODUCT_AAPT_CONFIG := xlarge mdpi

DEVICE_PACKAGE_OVERLAYS := \
    device/toshiba/betelgeuse/overlay

PRODUCT_TAGS += dalvik.gc.type-precise

PRODUCT_PACKAGES += \
    librs_jni \
    liba2dp \
    lights.tegra \
    com.android.future.usb.accessory \
    camera.tegra \
    libpkip \
    libaudioutils \
    tinyplay \
    tinycap \
    tinymix \
    sensors.tegra \
    libhuaweigeneric-ril \
    audio.primary.tegra \
    audio.a2dp.default \
    FolioParts \
    wmiconfig

# 3G
PRODUCT_PACKAGES += rild

# Filesystem management tools
PRODUCT_PACKAGES += \
    make_ext4fs

$(call inherit-product-if-exists, vendor/toshiba/betelgeuse/device-vendor.mk)
$(call inherit-product, frameworks/native/build/phone-hdpi-512-dalvik-heap.mk)
