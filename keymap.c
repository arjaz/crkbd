#include QMK_KEYBOARD_H
#include <stdio.h>
#include <keymap_steno.h>
#include "features/adaptive_keys.h"

#define ALPHA_LAYER 0
#define CYRILLIC_LAYER 1
#define NAVIGATION_LAYER 2
#define GAMING_LAYER 3
#define SYMBOL_LAYER 4
#define NUMPAD_LAYER 5
#define STENO_LAYER 6

enum {
    TD_ASTR_GUI,
    TD_LPAREN_ALT,
    TD_AMPR_SHIFT,
    TD_RPRN_CTRL,
    TD_Q_GRAVE,
    TD_W_QUES,
    TD_E_SLASH,
    TD_T_QUOT,
    TD_U_BSLS,
    TD_P_LBRC
};

enum macros_keycodes {
    SCROLL_LOCK_TG_CYRILLIC = SAFE_RANGE,
};

typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_adaptive_key(keycode, record)) {
        return false;
    }
    qk_tap_dance_action_t *action;

    switch (keycode) {
    case SCROLL_LOCK_TG_CYRILLIC:
        if (record->event.pressed) {
            SEND_STRING(SS_TAP(X_SCROLLLOCK));
            layer_invert(CYRILLIC_LAYER);
        }
        break;
    case TD(TD_ASTR_GUI) ... TD(TD_P_LBRC):
        action = &tap_dance_actions[TD_INDEX(keycode)];
        if (!record->event.pressed && action->state.count && !action->state.finished) {
            tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
            tap_code16(tap_hold->tap);
        }
        break;
    }
    return true;
}

void tap_dance_tap_hold_finished(qk_tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (state->pressed) {
        if (state->count == 1
#ifndef PERMISSIVE_HOLD
            && !state->interrupted
#endif
        ) {
            register_code16(tap_hold->hold);
            tap_hold->held = tap_hold->hold;
        } else {
            register_code16(tap_hold->tap);
            tap_hold->held = tap_hold->tap;
        }
    }
}

void tap_dance_tap_hold_reset(qk_tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (tap_hold->held) {
        unregister_code16(tap_hold->held);
        tap_hold->held = 0;
    }
}

#define ACTION_TAP_DANCE_TAP_HOLD(tap, hold) \
    { .fn = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset}, .user_data = (void *)&((tap_dance_tap_hold_t){tap, hold, 0}), }

qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_ASTR_GUI]    = ACTION_TAP_DANCE_TAP_HOLD(KC_ASTR, KC_LGUI),
    [TD_LPAREN_ALT]  = ACTION_TAP_DANCE_TAP_HOLD(KC_LPRN, KC_LALT),
    [TD_AMPR_SHIFT]  = ACTION_TAP_DANCE_TAP_HOLD(KC_AMPR, KC_LSFT),
    [TD_RPRN_CTRL]   = ACTION_TAP_DANCE_TAP_HOLD(KC_RPRN, KC_LCTL),
    [TD_Q_GRAVE]     = ACTION_TAP_DANCE_TAP_HOLD(KC_Q, KC_GRAVE),
    [TD_W_QUES]      = ACTION_TAP_DANCE_TAP_HOLD(KC_W, KC_QUES),
    [TD_E_SLASH]     = ACTION_TAP_DANCE_TAP_HOLD(KC_E, KC_SLASH),
    [TD_T_QUOT]      = ACTION_TAP_DANCE_TAP_HOLD(KC_T, KC_QUOT),
    [TD_U_BSLS]      = ACTION_TAP_DANCE_TAP_HOLD(KC_U, KC_BSLS),
    [TD_P_LBRC]      = ACTION_TAP_DANCE_TAP_HOLD(KC_P, KC_LBRC)
};

enum combo_events {
    COMBO_ESC,
    COMBO_TAB,
    COMBO_CYRILLIC,
    COMBO_STENO,
    COMBO_GAMING,
    COMBO_NAVIGATION,
    COMBO_CAPSWRD,
    COMBO_RALT,
    COMBO_COPY,
    COMBO_PASTE,
    COMBO_CUT,
    COMBO_LENGTH
};

const uint16_t PROGMEM combo_esc[] = {LGUI_T(KC_R), KC_T, COMBO_END};
const uint16_t PROGMEM combo_tab[] = {LGUI_T(KC_R), LCTL_T(KC_S), COMBO_END};
const uint16_t PROGMEM combo_cyrillic[] = {KC_P, KC_B, COMBO_END};
const uint16_t PROGMEM combo_ralt[] = {LALT_T(KC_N), LALT_T(KC_E), COMBO_END};
const uint16_t PROGMEM combo_capswrd[] = {KC_U, LT(SYMBOL_LAYER, KC_O), KC_Y, COMBO_END};
const uint16_t PROGMEM combo_steno[] = {KC_COMM, KC_Z, COMBO_END};
const uint16_t PROGMEM combo_gaming[] = {KC_Q, KC_J, COMBO_END};
const uint16_t PROGMEM combo_navigation[] = {KC_C, LT(SYMBOL_LAYER, KC_L), KC_D, COMBO_END};
const uint16_t PROGMEM combo_copy[] = {KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM combo_paste[] = {KC_C, LT(SYMBOL_LAYER, KC_L), COMBO_END};
const uint16_t PROGMEM combo_cut[] = {LT(SYMBOL_LAYER, KC_L), KC_D, COMBO_END};
combo_t key_combos[] = {
	[COMBO_ESC] = COMBO(combo_esc, KC_ESC),
    [COMBO_TAB] = COMBO(combo_tab, KC_TAB),
    [COMBO_CAPSWRD] = COMBO(combo_capswrd, CAPSWRD),
    [COMBO_RALT] = COMBO(combo_ralt, KC_RALT),
    [COMBO_COPY] = COMBO(combo_copy, LCTL(KC_C)),
    [COMBO_PASTE] = COMBO(combo_paste, LCTL(KC_V)),
    [COMBO_CUT] = COMBO(combo_cut, LCTL(KC_X)),
    /* Layer-related combos */
    [COMBO_CYRILLIC] = COMBO(combo_cyrillic, SCROLL_LOCK_TG_CYRILLIC),
    [COMBO_STENO] = COMBO(combo_steno, TG(STENO_LAYER)),
    [COMBO_GAMING] = COMBO(combo_gaming, TG(GAMING_LAYER)),
    [COMBO_NAVIGATION] = COMBO(combo_navigation, TG(NAVIGATION_LAYER)),
};
uint16_t COMBO_LEN = COMBO_LENGTH;

bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    if (layer_state_is(STENO_LAYER) && combo_index != COMBO_STENO)
        return false;

    if (layer_state_is(NAVIGATION_LAYER) && combo_index != COMBO_NAVIGATION) {
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
    }

    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [ALPHA_LAYER] = LAYOUT_split_3x6_3
    (KC_NO, KC_W, KC_F, KC_M, KC_P,  KC_V,
     KC_NO, KC_Z, KC_Q, KC_J, KC_NO, KC_NO,

     KC_NO,   LGUI_T(KC_R), LCTL_T(KC_S), LALT_T(KC_N), KC_T,         KC_B,
     KC_COMM, KC_A,         LALT_T(KC_E), LCTL_T(KC_I), LGUI_T(KC_H), KC_NO,

     KC_NO,  KC_X,                         KC_C, LT(SYMBOL_LAYER, KC_L), KC_D, KC_G,
     KC_DOT, KC_U, LT(SYMBOL_LAYER, KC_O), KC_Y,                         KC_K, KC_NO,

     KC_NO,         LT(NAVIGATION_LAYER, KC_SPC), LT(NUMPAD_LAYER, KC_BSPC),
     OSM(MOD_LSFT), KC_ENT,                       KC_NO
     ),

    [CYRILLIC_LAYER] = LAYOUT_split_3x6_3
    (KC_NO, TD(TD_Q_GRAVE), TD(TD_W_QUES), TD(TD_E_SLASH), KC_R,          TD(TD_T_QUOT),
     KC_Y,  TD(TD_U_BSLS),  KC_I,          KC_O,           TD(TD_P_LBRC), KC_NO,

     KC_NO, LGUI_T(KC_A), LCTL_T(KC_S), LALT_T(KC_D), KC_F,            KC_G,
     KC_H,  KC_J,         LALT_T(KC_K), LCTL_T(KC_L), LGUI_T(KC_SCLN), KC_NO,

     KC_NO, KC_Z, KC_X,                      LT(SYMBOL_LAYER, KC_C), KC_V,    KC_B,
     KC_N,  KC_M, LT(SYMBOL_LAYER, KC_COMM), KC_DOT,                 KC_RBRC, KC_TRNS,

     KC_TRNS, KC_TRNS, KC_TRNS,
     KC_TRNS, KC_TRNS, KC_TRNS
     ),

    [NAVIGATION_LAYER] = LAYOUT_split_3x6_3
    (KC_NO, KC_NO,      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO, KC_NO,
     KC_NO, KC_NO,      KC_WH_D, KC_MS_U, KC_WH_U, KC_HOME, KC_NO, KC_VOLD, KC_UP,   KC_VOLU, KC_NO, KC_NO,
     KC_NO, KC_MS_BTN3, KC_MS_L, KC_MS_D, KC_MS_R, KC_END,  KC_NO, KC_LEFT, KC_DOWN, KC_RGHT, KC_NO, KC_NO,

     KC_NO,      KC_MS_BTN1, KC_MS_BTN2,
     KC_MS_BTN1, KC_MS_BTN2, KC_NO
     ),

    [GAMING_LAYER] = LAYOUT_split_3x6_3
    (KC_TAB,   KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I,    KC_O,   KC_P,    KC_LBRC,
     KC_LSFT,  KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K,    KC_L,   KC_SCLN, KC_QUOT,
     KC_LCTRL, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_RBRC, KC_TRNS,
     KC_LALT,  KC_SPC,  KC_TRNS,
     KC_TRNS,  KC_TRNS, KC_TRNS
     ),

    [SYMBOL_LAYER] = LAYOUT_split_3x6_3
    (KC_NO, KC_NO, KC_LT, KC_GT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,

     KC_NO,   LGUI_T(KC_MINS), LCTL_T(KC_QUOT),   LALT_T(KC_SCLN),  KC_EQL,          KC_PERC,
     KC_CIRC, KC_AMPR,         TD(TD_LPAREN_ALT), TD(TD_RPRN_CTRL), TD(TD_ASTR_GUI), KC_NO,

     KC_NO, KC_DLR, KC_GRAVE, KC_SLASH, KC_BSLS, KC_AT, KC_EXLM, KC_LBRC, KC_LCBR, KC_RCBR, KC_RBRC, KC_NO,

     KC_TRNS, KC_HASH, KC_TRNS,
     KC_TRNS, KC_TRNS, KC_TRNS
     ),

    [NUMPAD_LAYER] = LAYOUT_split_3x6_3
    (KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,

     KC_NO, LGUI_T(KC_5), LCTL_T(KC_6), LALT_T(KC_7), KC_8, KC_9, KC_NO, KC_F4, LALT_T(KC_F5), LCTL_T(KC_F6), LGUI_T(KC_F11), KC_NO,
     KC_NO, KC_0,         KC_1,         KC_2,         KC_3, KC_4, KC_NO, KC_F1, KC_F2,         KC_F3,         KC_F12,         KC_NO,

     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
     ),

    [STENO_LAYER] = LAYOUT_split_3x6_3
    (KC_NO,  STN_N1, STN_N2, STN_N3, STN_N4, STN_N5,
     STN_N6, STN_N7, STN_N8, STN_N9, STN_N9, STN_N9,

     KC_NO,   STN_S1, STN_TL, STN_PL, STN_HL, STN_ST1,
     STN_ST3, STN_FR, STN_PR, STN_LR, STN_TR, STN_DR,

     KC_NO,   STN_S2, STN_KL, STN_WL, STN_RL, STN_ST2,
     STN_ST4, STN_RR, STN_BR, STN_GR, STN_SR, STN_ZR,

     STN_N1, STN_A, STN_O,
     STN_E,  STN_U, STN_N1
     )
};
