#include QMK_KEYBOARD_H
#include <stdio.h>

enum {
    TD_AT_CTRL,
    TD_HASH_ALT,
    TD_ASTR_ALT,
    TD_LPAREN_CTRL,
    TD_EQL_PLUS,
    TD_GRAVE_TILDE,
    TD_SLASH_QUES,
    TD_BSLS_PIPE,
    TD_Q_GRAVE,
    TD_W_QUES,
    TD_E_SLASH,
    TD_T_RALT_T,
    TD_U_RALT_U,
    TD_RALT_S_S,
    TD_RALT_QUOT_QUOT,
    TD_M_RBRC,
};

enum macros_keycodes {
    /* Scroll lock and toggle layer 1 */
    SCROLL_LOCK_TG_1 = SAFE_RANGE,
};

typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    qk_tap_dance_action_t *action;

    switch (keycode) {
    case SCROLL_LOCK_TG_1:
        if (record->event.pressed) {
            SEND_STRING(SS_TAP(X_SCROLLLOCK));
            layer_invert(1);
        }
        break;
    case TD(TD_AT_CTRL) ... TD(TD_M_RBRC):
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
    [TD_AT_CTRL]        = ACTION_TAP_DANCE_TAP_HOLD(KC_AT, KC_LCTL),
    [TD_HASH_ALT]       = ACTION_TAP_DANCE_TAP_HOLD(KC_HASH, KC_LALT),
    [TD_ASTR_ALT]       = ACTION_TAP_DANCE_TAP_HOLD(KC_ASTR, KC_LALT),
    [TD_LPAREN_CTRL]    = ACTION_TAP_DANCE_TAP_HOLD(KC_LPRN, KC_LCTL),
    [TD_Q_GRAVE]        = ACTION_TAP_DANCE_TAP_HOLD(KC_Q, KC_GRAVE),
    [TD_W_QUES]         = ACTION_TAP_DANCE_TAP_HOLD(KC_W, KC_QUES),
    [TD_E_SLASH]        = ACTION_TAP_DANCE_TAP_HOLD(KC_E, KC_SLASH),
    [TD_T_RALT_T]       = ACTION_TAP_DANCE_TAP_HOLD(KC_T, RALT(KC_T)),
    [TD_U_RALT_U]       = ACTION_TAP_DANCE_TAP_HOLD(KC_U, RALT(KC_U)),
    [TD_RALT_S_S]       = ACTION_TAP_DANCE_TAP_HOLD(RALT(KC_S), KC_S),
    [TD_RALT_QUOT_QUOT] = ACTION_TAP_DANCE_TAP_HOLD(RALT(KC_QUOT), KC_QUOT),
    [TD_M_RBRC]         = ACTION_TAP_DANCE_TAP_HOLD(KC_M, KC_RBRC),
    [TD_EQL_PLUS]       = ACTION_TAP_DANCE_TAP_HOLD(KC_EQL, KC_PLUS),
    [TD_GRAVE_TILDE]    = ACTION_TAP_DANCE_TAP_HOLD(KC_GRAVE, KC_TILDE),
    [TD_SLASH_QUES]     = ACTION_TAP_DANCE_TAP_HOLD(KC_SLASH, KC_QUES),
    [TD_BSLS_PIPE]      = ACTION_TAP_DANCE_TAP_HOLD(KC_BSLS, KC_PIPE)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base dvorak layer */
	/* TODO:
	   I don't like how shift is handled that much
	   Also, backspace can be better
	*/
    [0] = LAYOUT_split_3x6_3
    (KC_NO, KC_QUOT, KC_COMM, KC_DOT, KC_P, KC_Y,
     KC_F,  KC_G,    KC_C,    KC_R,   KC_L, KC_NO,

     CAPSWRD, LGUI_T(KC_A), LCTL_T(KC_O), LALT_T(KC_E), LSFT_T(KC_U), KC_I,
     KC_D,    LSFT_T(KC_H), LALT_T(KC_T), LCTL_T(KC_N), LGUI_T(KC_S), KC_NO,

     SCROLL_LOCK_TG_1, KC_SCLN, KC_Q, KC_J, KC_K, KC_X,
     KC_B,             KC_M,    KC_W, KC_V, KC_Z, OSM(MOD_RALT),

     KC_BSPC,        KC_SPC,        LT(5, KC_ESC),
     LT(4, KC_TAB),  LT(2, KC_ENT), KC_NO
     ),

    /* TODO: figure out how to send unicode */
    /* TODO: fix somehow that u/ralt(u) conflicting with the ctrl mod */
    /* Cyrillic layer */
    [1] = LAYOUT_split_3x6_3
    (KC_TRNS, TD(TD_Q_GRAVE),  TD(TD_W_QUES), TD(TD_E_SLASH), KC_R, TD(TD_T_RALT_T),
     KC_Y,    TD(TD_U_RALT_U), KC_I,          KC_O,           KC_P, KC_LBRC,

     KC_TRNS, LGUI_T(KC_A), TD(TD_RALT_S_S), LALT_T(KC_D), LSFT_T(KC_F),    KC_G,
     KC_H,    LSFT_T(KC_J), LALT_T(KC_K),    LCTL_T(KC_L), LGUI_T(KC_SCLN), TD(TD_RALT_QUOT_QUOT),

     SCROLL_LOCK_TG_1, KC_Z,          KC_X,    KC_C,   KC_V,          KC_B,
     KC_N,             TD(TD_M_RBRC), KC_COMM, KC_DOT, RALT(KC_RBRC), KC_TRNS,

     KC_TRNS, KC_TRNS, KC_TRNS,
     KC_TRNS, KC_TRNS, KC_TRNS
     ),

    /* Navigation layer */
    [2] = LAYOUT_split_3x6_3
    (KC_NO, KC_NO, KC_WH_D, KC_MS_U, KC_WH_U, KC_VOLU, KC_NO, KC_NO,   KC_UP,   KC_NO,   KC_NO, KC_NO,
     KC_NO, KC_NO, KC_MS_L, KC_MS_D, KC_MS_R, KC_VOLD, KC_NO, KC_LEFT, KC_DOWN, KC_RGHT, KC_NO, KC_NO,
     KC_NO, KC_NO, KC_NO,   KC_HOME, KC_END,  KC_MUTE, KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO, KC_NO,

     KC_MS_BTN1, KC_MS_BTN3, KC_MS_BTN2,
     KC_NO,      KC_NO,      KC_NO
     ),

    /* Qwerty layer */
    /* TODO: place the mods */
    [3] = LAYOUT_split_3x6_3
    (KC_LSFT, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I,    KC_O,   KC_P,    KC_LBRC,
     KC_TAB,  KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K,    KC_L,   KC_SCLN, KC_QUOT,
     TO(0),   KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_RBRC, KC_TRNS,
     KC_TRNS, KC_TRNS, KC_TRNS,
     KC_TRNS, KC_TRNS, KC_TRNS
     ),

    /* Symbols layer */
    [4] = LAYOUT_split_3x6_3
    (KC_NO, KC_1, KC_2, KC_3, KC_4, KC_5,
     KC_6,  KC_7, KC_8, KC_9, KC_0, KC_NO,

     KC_NO,   KC_EXLM, TD(TD_AT_CTRL),  TD(TD_HASH_ALT),    KC_DLR,  KC_PERC,
     KC_CIRC, KC_AMPR, TD(TD_ASTR_ALT), TD(TD_LPAREN_CTRL), KC_RPRN, KC_NO,

     TO(3),            TD(TD_EQL_PLUS), TD(TD_GRAVE_TILDE), TD(TD_SLASH_QUES), KC_MINS, KC_UNDS,
     TD(TD_BSLS_PIPE), KC_LCBR,         KC_RCBR,            KC_LBRC,           KC_RBRC, KC_NO,

     KC_DEL, KC_TRNS, KC_TRNS,
     KC_TRNS,  KC_TRNS, KC_TRNS
     ),

    /* Fn-keys + numpad layer */
    [5] = LAYOUT_split_3x6_3
    (KC_NO, KC_NO, KC_7,  KC_8,  KC_9,   KC_NO,
     KC_NO, KC_F7, KC_F8, KC_F9, KC_F10, KC_NO,

     KC_NO, KC_0,  KC_4,  KC_5,  KC_6,   KC_NO,
     KC_NO, KC_F4, KC_F5, KC_F6, KC_F11, KC_NO,

     KC_NO, KC_NO, KC_1,  KC_2,  KC_3,   KC_NO,
     KC_NO, KC_F1, KC_F2, KC_F3, KC_F12, KC_NO,

     KC_NO, KC_NO, KC_NO,
     KC_NO, KC_NO, KC_NO
     )
};
