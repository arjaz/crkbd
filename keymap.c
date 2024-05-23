#include QMK_KEYBOARD_H
#include <stdio.h>
#include <keymap_ukrainian.h>
#include "features/tap_hold_dance.h"
#include "features/achordion.h"

#define ALPHA_LAYER 0
#define CYRILLIC_LAYER 1
#define NAVIGATION_LAYER1 2
#define NAVIGATION_LAYER2 3
#define GAMING_LAYER 4
#define SYMBOL_LAYER 5
#define NUMBER_LAYER 6
#define FN_LAYER 7

#define AR_J KC_J
#define AR_G KC_G
#define AR_M LT(NUMBER_LAYER, KC_M)
#define AR_P KC_P
#define AR_V KC_V
#define AR_ASTR KC_ASTR
#define AR_MINS KC_MINS
#define AR_QUOT LT(NUMBER_LAYER, KC_QUOT)
#define AR_EQL KC_EQL
#define AR_HASH KC_HASH
#define AR_C KC_C
#define AR_S LCTL_T(KC_S)
#define AR_N LALT_T(KC_N)
#define AR_T LGUI_T(KC_T)
#define AR_K KC_K
#define AR_COMM KC_COMM
#define AR_A LGUI_T(KC_A)
#define AR_E LALT_T(KC_E)
#define AR_I LCTL_T(KC_I)
#define AR_H KC_H
#define AR_X KC_X
#define AR_F KC_F
#define AR_L LT(SYMBOL_LAYER, KC_L)
#define AR_D LT(NAVIGATION_LAYER1, KC_D)
#define AR_W KC_W
#define AR_DOT KC_DOT
#define AR_U LT(NAVIGATION_LAYER2, KC_U)
#define AR_O LT(SYMBOL_LAYER, KC_O)
#define AR_Y KC_Y
#define AR_B KC_B
#define AR_R KC_R
#define AR_BSPC KC_BSPC
#define AR_LSFT OSM(MOD_LSFT)
#define AR_SPC KC_SPC

#define AR_F5 LGUI_T(KC_F5)

enum {
    TD_T_QUOT,
    TD_U_BSLS,
    TD_P_LBRC
};

enum macros_keycodes {
    SCROLL_LOCK_TG_CYRILLIC = SAFE_RANGE,
};

uint16_t achordion_streak_timeout(uint16_t tap_hold_keycode) {
    return 150;
}

uint16_t achordion_timeout(uint16_t tap_hold_keycode) {
    return 500;
}

bool achordion_chord(uint16_t tap_hold_keycode,
                     keyrecord_t* tap_hold_record,
                     uint16_t other_keycode,
                     keyrecord_t* other_record) {
    return true;
}

void matrix_scan_user(void) {
    achordion_task();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_achordion(keycode, record)) { return false; }
    tap_dance_action_t *action;

    switch (keycode) {
    case SCROLL_LOCK_TG_CYRILLIC:
        if (record->event.pressed) {
            tap_code(KC_SCROLL_LOCK);
            layer_invert(CYRILLIC_LAYER);
        }
        break;
    case TD(TD_T_QUOT) ... TD(TD_P_LBRC):
        action = &tap_dance_actions[TD_INDEX(keycode)];
        if (!record->event.pressed && action->state.count && !action->state.finished) {
            tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
            tap_code16(tap_hold->tap);
        }
        break;
    }
    return true;
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_T_QUOT]  = ACTION_TAP_DANCE_TAP_HOLD(KC_T, KC_QUOT),
    [TD_U_BSLS]  = ACTION_TAP_DANCE_TAP_HOLD(KC_U, KC_BSLS),
    [TD_P_LBRC]  = ACTION_TAP_DANCE_TAP_HOLD(KC_P, KC_LBRC),
};

enum combo_events {
    COMBO_ESC,
    COMBO_TAB,
    COMBO_ENTER,
    COMBO_CAPSWRD,
    COMBO_CYRILLIC,
    COMBO_GAMING,
    COMBO_NAVIGATION,
    COMBO_RALT,
    COMBO_COPY,
    COMBO_PASTE,
    COMBO_Z,
    COMBO_Q,
    COMBO_SLASH,
    COMBO_LENGTH
};

const uint16_t PROGMEM combo_z[] = {AR_F, AR_D, COMBO_END};
const uint16_t PROGMEM combo_q[] = {AR_G, AR_P, COMBO_END};

const uint16_t PROGMEM combo_slash[] = {AR_QUOT, AR_EQL, COMBO_END};

const uint16_t PROGMEM combo_esc[] = {AR_X, AR_D, COMBO_END};
const uint16_t PROGMEM combo_tab[] = {AR_C, AR_S, COMBO_END};
const uint16_t PROGMEM combo_enter[] = {AR_Y, AR_B, COMBO_END};
const uint16_t PROGMEM combo_ralt[] = {AR_N, AR_E, COMBO_END};

const uint16_t PROGMEM combo_capswrd[] = {AR_LSFT, AR_BSPC, COMBO_END};
const uint16_t PROGMEM combo_cyrillic[] = {AR_V, AR_ASTR, COMBO_END};
const uint16_t PROGMEM combo_gaming[] = {AR_U, AR_E, AR_Y, COMBO_END};
const uint16_t PROGMEM combo_navigation[] = {AR_F, AR_N, AR_D, COMBO_END};

const uint16_t PROGMEM combo_copy[] = {AR_X, AR_F, AR_L, COMBO_END};
const uint16_t PROGMEM combo_paste[] = {AR_X, AR_F, AR_L, AR_D, COMBO_END};

combo_t key_combos[] = {
    /* Missing important control keys */
	[COMBO_ESC] = COMBO(combo_esc, KC_ESC),
    [COMBO_TAB] = COMBO(combo_tab, KC_TAB),
    [COMBO_ENTER] = COMBO(combo_enter, KC_ENT),
    [COMBO_CAPSWRD] = COMBO(combo_capswrd, CW_TOGG),
    [COMBO_RALT] = COMBO(combo_ralt, KC_RALT),
    /* Useful shortcuts */
    [COMBO_COPY] = COMBO(combo_copy, LCTL(KC_C)),
    [COMBO_PASTE] = COMBO(combo_paste, LCTL(KC_V)),
    /* Layer-switching combos */
    [COMBO_CYRILLIC] = COMBO(combo_cyrillic, SCROLL_LOCK_TG_CYRILLIC),
    [COMBO_GAMING] = COMBO(combo_gaming, TG(GAMING_LAYER)),
    [COMBO_NAVIGATION] = COMBO(combo_navigation, TG(NAVIGATION_LAYER2)),
    /* Missing keys */
    [COMBO_Z] = COMBO(combo_z, KC_Z),
    [COMBO_Q] = COMBO(combo_q, KC_Q),
    [COMBO_SLASH] = COMBO(combo_slash, KC_SLASH),
};
uint16_t COMBO_LEN = COMBO_LENGTH;

bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    if (layer_state_is(NAVIGATION_LAYER2) && combo_index != COMBO_NAVIGATION) {
        return false;
    }

    if (layer_state_is(SYMBOL_LAYER)) {
        return false;
    }

    switch (combo_index) {
    case COMBO_CYRILLIC:
        if (!(layer_state_is(ALPHA_LAYER) || layer_state_is(CYRILLIC_LAYER))) {
            return false;
        }
        break;
    case COMBO_GAMING:
        if (!(layer_state_is(ALPHA_LAYER) || layer_state_is(GAMING_LAYER))) {
            return false;
        }
        break;
    case COMBO_Z:
    case COMBO_Q:
        if (!layer_state_is(ALPHA_LAYER)) {
            return false;
        }
        break;
    }

    return true;
}

const key_override_t dot_key_override = ko_make_with_layers(MOD_MASK_SHIFT, KC_DOT, S(KC_SCLN), 1);
const key_override_t comm_key_override = ko_make_with_layers(MOD_MASK_SHIFT, KC_COMM, KC_SCLN, 1);
const key_override_t slash_key_override = ko_make_with_layers(MOD_MASK_SHIFT, KC_SLASH, KC_BSLS, 1);

const key_override_t **key_overrides = (const key_override_t *[]){
    &dot_key_override,
    &comm_key_override,
    &slash_key_override,
    NULL
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [ALPHA_LAYER] = LAYOUT_split_3x5_3
    (AR_J,    AR_G,    AR_M,    AR_P,   AR_V,
     AR_ASTR, AR_MINS, AR_QUOT, AR_EQL, AR_HASH,

     AR_C,    AR_S, AR_N, AR_T, AR_K,
     AR_COMM, AR_A, AR_E, AR_I, AR_H,

     AR_X,   AR_F, AR_L, AR_D, AR_W,
     AR_DOT, AR_U, AR_O, AR_Y, AR_B,

     KC_NO,   AR_R,   AR_BSPC,
     AR_LSFT, AR_SPC, KC_NO
     ),

    [CYRILLIC_LAYER] = LAYOUT_split_3x5_3
    (KC_Q, KC_W,          LT(NUMBER_LAYER, KC_E), KC_R, TD(TD_T_QUOT),
     KC_Y, TD(TD_U_BSLS), LT(NUMBER_LAYER, KC_I), KC_O, TD(TD_P_LBRC),

     KC_A, LCTL_T(KC_S), LALT_T(KC_D), LGUI_T(KC_F), KC_G,
     KC_H, LGUI_T(KC_J), LALT_T(KC_K), LCTL_T(KC_L), KC_SCLN,

     KC_Z, KC_X,                        LT(SYMBOL_LAYER, KC_C),    LT(NAVIGATION_LAYER1, KC_V), KC_B,
     KC_N, LT(NAVIGATION_LAYER2, KC_M), LT(SYMBOL_LAYER, KC_COMM), KC_DOT,   KC_RBRC,

     KC_TRNS, KC_TRNS, KC_TRNS,
     KC_TRNS, KC_TRNS, KC_TRNS
     ),

    [NAVIGATION_LAYER1] = LAYOUT_split_3x5_3
    (KC_NO, KC_NO,   KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,
     KC_NO, KC_LCTL, KC_LALT, KC_NO, KC_NO, KC_NO, KC_VOLD, KC_UP,   KC_VOLU, KC_NO,
     KC_NO, KC_NO,   KC_NO,   KC_NO, KC_NO, KC_NO, KC_LEFT, KC_DOWN, KC_RGHT, KC_NO,

     KC_NO,   KC_MS_BTN1, KC_MS_BTN2,
     KC_PGDN, KC_PGUP,    KC_NO
     ),

    [NAVIGATION_LAYER2] = LAYOUT_split_3x5_3
    (KC_NO,      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,
     KC_NO,      KC_WH_D, KC_MS_U, KC_WH_U, KC_HOME, KC_NO, KC_VOLD, KC_UP,   KC_VOLU, KC_NO,
     KC_MS_BTN3, KC_MS_L, KC_MS_D, KC_MS_R, KC_END,  KC_NO, KC_LEFT, KC_DOWN, KC_RGHT, KC_NO,

     KC_NO,   KC_MS_BTN1, KC_MS_BTN2,
     KC_PGDN, KC_PGUP,    KC_NO
     ),

    [GAMING_LAYER] = LAYOUT_split_3x5_3
    (KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I,    KC_O,   KC_P,
     KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K,    KC_L,   LT(NUMBER_LAYER, KC_SCLN),
     LCTL_T(KC_Z), LALT_T(KC_X), KC_C, KC_V, LT(NUMBER_LAYER, KC_B), KC_N, KC_M, KC_COMM, KC_DOT, KC_RBRC,
     KC_NO,   KC_SPC,  KC_LSFT,
     KC_TRNS, KC_TRNS, KC_NO
     ),

    [SYMBOL_LAYER] = LAYOUT_split_3x5_3
    (KC_NO,   KC_NO,   KC_DLR,  KC_NO,   KC_NO, KC_TRNS, KC_TRNS,  KC_GRAVE, KC_TRNS,    KC_TRNS,
     KC_LT,   KC_LPRN, KC_RPRN, KC_GT,   KC_NO, KC_TRNS, KC_TILDE, KC_QUES,  KC_AMPR,    KC_EXLM,
     KC_LBRC, KC_LCBR, KC_RCBR, KC_RBRC, KC_NO, KC_TRNS, KC_AT,    KC_PERC,  S(KC_BSLS), KC_CIRC,

     KC_TRNS, KC_PERC, KC_SLASH,
     KC_TRNS, KC_TRNS, KC_TRNS
     ),

    [NUMBER_LAYER] = LAYOUT_split_3x5_3
    (KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_F9, KC_F10, KC_F11, KC_F12,
     KC_8,  KC_7,  KC_6,  KC_5,  KC_NO, KC_TRNS, AR_F5, KC_F6,  KC_F7,  KC_F8,
     KC_4,  KC_3,  KC_2,  KC_1,  KC_0,  KC_TRNS, KC_F1, KC_F2,  KC_F3,  KC_F4,

     KC_TRNS, KC_9, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
     ),
};
