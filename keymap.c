#include QMK_KEYBOARD_H
#include <stdio.h>
#include <keymap_steno.h>

#define ALPHA_LAYER 0
#define HANDS_DOWN_LAYER 1
#define CYRILLIC_LAYER 2
#define NAVIGATION_LAYER 3
#define GAMING_LAYER 4
#define SYMBOL_LAYER 5
#define NUMPAD_LAYER 6
#define STENO_LAYER 7

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
    TD_P_LBRC,
    TD_BSPC_C_BSPC
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
    qk_tap_dance_action_t *action;

    switch (keycode) {
    case SCROLL_LOCK_TG_CYRILLIC:
        if (record->event.pressed) {
            SEND_STRING(SS_TAP(X_SCROLLLOCK));
            layer_invert(CYRILLIC_LAYER);
        }
        break;
    case TD(TD_ASTR_GUI) ... TD(TD_BSPC_C_BSPC):
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
    [TD_P_LBRC]      = ACTION_TAP_DANCE_TAP_HOLD(KC_P, KC_LBRC),
    [TD_BSPC_C_BSPC] = ACTION_TAP_DANCE_TAP_HOLD(KC_BSPC, LCTL(KC_BSPC))
};

enum combo_events {
    COMBO_ESC,
    COMBO_CYRILLIC,
    COMBO_STENO,
    COMBO_GAMING,
    COMBO_HANDS_DOWN,
    COMBO_CAPSWRD,
    COMBO_COPY,
    COMBO_PASTE,
    COMBO_CUT,
    COMBO_LENGTH
};

const uint16_t PROGMEM combo_comm_dot[] = {KC_COMM, KC_DOT, COMBO_END};
const uint16_t PROGMEM combo_p_y[]      = {KC_P, KC_Y, COMBO_END};
const uint16_t PROGMEM combo_u_h[]      = {LSFT_T(KC_U), LSFT_T(KC_H), COMBO_END};
const uint16_t PROGMEM combo_f_g[]      = {KC_F, KC_G, COMBO_END};
const uint16_t PROGMEM combo_c_r[]      = {KC_C, KC_R, COMBO_END};
const uint16_t PROGMEM combo_quot_r[]   = {KC_COMM, KC_R, COMBO_END};
const uint16_t PROGMEM combo_scln_q[]   = {KC_SCLN, KC_Q, COMBO_END};
const uint16_t PROGMEM combo_q_j[]      = {KC_Q, KC_J, COMBO_END};
const uint16_t PROGMEM combo_j_k[]      = {KC_J, KC_K, COMBO_END};
combo_t key_combos[] = {
	[COMBO_ESC] = COMBO(combo_comm_dot, KC_ESC),
    [COMBO_CAPSWRD] = COMBO(combo_u_h, CAPSWRD),
    [COMBO_COPY] = COMBO(combo_scln_q, LCTL(KC_C)),
    [COMBO_PASTE] = COMBO(combo_q_j, LCTL(KC_V)),
    [COMBO_CUT] = COMBO(combo_j_k, LCTL(KC_X)),
    /* Layer-related combos */
    [COMBO_CYRILLIC] = COMBO(combo_p_y, SCROLL_LOCK_TG_CYRILLIC),
    [COMBO_STENO] = COMBO(combo_f_g, TG(STENO_LAYER)),
    [COMBO_GAMING] = COMBO(combo_c_r, TG(GAMING_LAYER)),
    [COMBO_HANDS_DOWN] = COMBO(combo_quot_r, TG(HANDS_DOWN_LAYER)),
};
uint16_t COMBO_LEN = COMBO_LENGTH;

bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    if (layer_state_is(STENO_LAYER) && combo_index != COMBO_STENO)
        return false;

    if (layer_state_is(NAVIGATION_LAYER)) {
        return false;
    }

    switch (combo_index) {
    case COMBO_CYRILLIC:
        if (!(layer_state_is(ALPHA_LAYER) || layer_state_is(HANDS_DOWN_LAYER) || layer_state_is(CYRILLIC_LAYER))) {
            return false;
        }
        break;
    case COMBO_GAMING:
        if (!(layer_state_is(ALPHA_LAYER) || layer_state_is(HANDS_DOWN_LAYER) || layer_state_is(GAMING_LAYER))) {
            return false;
        }
        break;
    case COMBO_HANDS_DOWN:
        if (!(layer_state_is(ALPHA_LAYER) || layer_state_is(HANDS_DOWN_LAYER))) {
            return false;
        }
        break;
    }

    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [ALPHA_LAYER] = LAYOUT_split_3x6_3
    (KC_NO, KC_QUOT, KC_COMM, KC_DOT, KC_P, KC_Y,
     KC_F,  KC_G,    KC_C,    KC_R,   KC_L, KC_NO,

     KC_NO, LGUI_T(KC_A), LCTL_T(KC_O), LALT_T(KC_E), LSFT_T(KC_U), KC_I,
     KC_D,  LSFT_T(KC_H), LALT_T(KC_T), LCTL_T(KC_N), LGUI_T(KC_S), KC_NO,

     KC_NO, KC_SCLN, KC_Q, KC_J, KC_K, KC_X,
     KC_B,  KC_M,    KC_W, KC_V, KC_Z, KC_NO,

     KC_NO,                        KC_SPC,                   LT(NUMPAD_LAYER, KC_BSPC),
     LT(NAVIGATION_LAYER, KC_TAB), LT(SYMBOL_LAYER, KC_ENT), KC_NO
     ),

    [HANDS_DOWN_LAYER] = LAYOUT_split_3x6_3
    (KC_NO, KC_W, KC_F, KC_M, KC_P,  KC_V,
     KC_NO, KC_Z, KC_Q, KC_J, KC_NO, KC_NO,

     KC_NO,   LGUI_T(KC_R), LCTL_T(KC_S), LALT_T(KC_N), LSFT_T(KC_T), KC_B,
     KC_COMM, LSFT_T(KC_A), LALT_T(KC_E), LCTL_T(KC_I), LGUI_T(KC_H), KC_NO,

     KC_NO,  KC_X, KC_C, KC_L, KC_D, KC_G,
     KC_DOT, KC_U, KC_O, KC_Y, KC_K, KC_NO,

     KC_TRNS, KC_TRNS, KC_TRNS,
     KC_TRNS, KC_TRNS, KC_TRNS
     ),

    [CYRILLIC_LAYER] = LAYOUT_split_3x6_3
    (KC_NO, TD(TD_Q_GRAVE), TD(TD_W_QUES), TD(TD_E_SLASH), KC_R,          TD(TD_T_QUOT),
     KC_Y,  TD(TD_U_BSLS),  KC_I,          KC_O,           TD(TD_P_LBRC), KC_NO,

     KC_NO, LGUI_T(KC_A), LCTL_T(KC_S), LALT_T(KC_D), LSFT_T(KC_F),    KC_G,
     KC_H,  LSFT_T(KC_J), LALT_T(KC_K), LCTL_T(KC_L), LGUI_T(KC_SCLN), KC_NO,

     KC_NO, KC_Z, KC_X,    KC_C,   KC_V,    KC_B,
     KC_N,  KC_M, KC_COMM, KC_DOT, KC_RBRC, KC_TRNS,

     KC_TRNS, KC_TRNS, KC_TRNS,
     KC_TRNS, KC_TRNS, KC_TRNS
     ),

    [NAVIGATION_LAYER] = LAYOUT_split_3x6_3
    (KC_NO, KC_NO,      KC_WH_D, KC_MS_U, KC_WH_U, KC_NO, KC_NO, KC_VOLD, KC_UP,   KC_VOLU, KC_NO, KC_NO,
     KC_NO, KC_MS_BTN3, KC_MS_L, KC_MS_D, KC_MS_R, KC_NO, KC_NO, KC_LEFT, KC_DOWN, KC_RGHT, KC_NO, KC_NO,
     KC_NO, KC_NO,      KC_HOME, KC_NO,   KC_END,  KC_NO, KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO, KC_NO,

     KC_NO, KC_MS_BTN1, KC_MS_BTN2,
     KC_NO, KC_NO,      KC_NO
     ),

    /* TODO: remove stuff from the outer columns */
    [GAMING_LAYER] = LAYOUT_split_3x6_3
    (KC_LSFT,  KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I,    KC_O,   KC_P,    KC_LBRC,
     KC_TAB,   KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K,    KC_L,   KC_SCLN, KC_QUOT,
     KC_LCTRL, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_RBRC, KC_TRNS,
     KC_LALT,  KC_TRNS, KC_TRNS,
     KC_TRNS,  KC_TRNS, KC_TRNS
     ),

    [SYMBOL_LAYER] = LAYOUT_split_3x6_3
    (KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,

     KC_NO,   LGUI_T(KC_SCLN),   LCTL_T(KC_MINS),   LALT_T(KC_EQL),   LSFT_T(KC_QUOT), KC_PERC,
     KC_CIRC, TD(TD_AMPR_SHIFT), TD(TD_LPAREN_ALT), TD(TD_RPRN_CTRL), TD(TD_ASTR_GUI), KC_NO,

     KC_NO, KC_DLR, KC_GRAVE, KC_SLASH, KC_BSLS, KC_AT, KC_EXLM, KC_LBRC, KC_LCBR, KC_RCBR, KC_RBRC, KC_NO,

     KC_TRNS, KC_HASH, KC_DEL, KC_TRNS, KC_TRNS, KC_TRNS
     ),

    [NUMPAD_LAYER] = LAYOUT_split_3x6_3
    (KC_NO, KC_NO, KC_7, KC_8,  KC_9, KC_NO, KC_NO, KC_F7, KC_F8, KC_F9, KC_F10, KC_NO,

     KC_NO, LGUI_T(KC_0),  LCTL_T(KC_4),  LALT_T(KC_5),  LSFT_T(KC_6),   KC_NO,
     KC_NO, LSFT_T(KC_F4), LALT_T(KC_F5), LCTL_T(KC_F6), LGUI_T(KC_F11), KC_NO,

     KC_NO, KC_NO, KC_1, KC_2,  KC_3, KC_NO, KC_NO, KC_F1, KC_F2, KC_F3, KC_F12, KC_NO,
     KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
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
