$(call inherit-product, device/toshiba/betelgeuse/full_betelgeuse.mk)

# Inherit some common CM stuff.
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

# Inherit some common CM stuff.
$(call inherit-product, vendor/cm/config/gsm.mk)

PRODUCT_BUILD_PROP_OVERRIDES += PRODUCT_MODEL=GT-P7510 PRODUCT_BRAND=samsung PRODUCT_MANUFACTURER=samsung PRODUCT_NAME=GT-P7510 TARGET_DEVICE=GT-P7510 BUILD_ID=HTJ85B BUILD_FINGERPRINT=samsung/GT-P7510/GT-P7510:3.2/HTJ85B/UEKMM:user/release-keys PRIVATE_BUILD_DESC="GT-P7510-user 3.2 HTJ85B UEKMM release-keys"

PRODUCT_NAME := cm_betelgeuse
PRODUCT_RELEASE_NAME := betelgeuse
PRODUCT_DEVICE := betelgeuse
PRODUCT_BRAND := toshiba
PRODUCT_MODEL := folio100
PRODUCT_MANUFACTURER := toshiba
