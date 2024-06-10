MOUSEKEY_ENABLE     = yes
LTO_ENABLE          = yes
TAP_DANCE_ENABLE    = yes
CAPS_WORD_ENABLE    = yes
COMBO_ENABLE        = yes
RGBLIGHT_ENABLE     = no
BOOTLOADER          = caterina
KEY_OVERRIDE_ENABLE = yes
DEBOUNCE_TYPE       = asym_eager_defer_pk

SRC += features/tap_hold_dance.c
SRC += features/achordion.c
