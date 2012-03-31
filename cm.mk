$(call inherit-product, device/toshiba/betelgeuse/full_betelgeuse.mk)

# Inherit some common CM stuff.
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

# Inherit some common CM stuff.
$(call inherit-product, vendor/cm/config/gsm.mk)

PRODUCT_BUILD_PROP_OVERRIDES += PRODUCT_NAME=betelgeuse BUILD_ID=IML77 BUILD_DISPLAY_ID=IML77 BUILD_FINGERPRINT="motorola/tervigon/wingray:4.0.3/IML77/239789:user/release-keys" PRIVATE_BUILD_DESC="tervigon-user 4.0.3 IML77 239789 release-keys"

PRODUCT_NAME := cm_betelgeuse
PRODUCT_RELEASE_NAME := betelgeuse
PRODUCT_DEVICE := betelgeuse
PRODUCT_BRAND := toshiba
PRODUCT_MODEL := folio100
PRODUCT_MANUFACTURER := toshiba

TARGET_BOOTANIMATION_NAME := horizontal-1024x600
