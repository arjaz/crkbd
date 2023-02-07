#include QMK_KEYBOARD_H
#include <stdio.h>
#include <keymap_steno.h>

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
    TD_T_RALT_QUOT,
    TD_T_RALT_T,
    TD_U_RALT_U,
    TD_P_LBRC,
    TD_RALT_S_S,
    TD_RALT_QUOT_QUOT,
    TD_M_RBRC,
    TD_BSPC_C_BSPC
};

enum macros_keycodes {
    /* Scroll lock and toggle layer 1 */
    SCROLL_LOCK_TG_CYR = SAFE_RANGE,
};

typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    qk_tap_dance_action_t *action;

    switch (keycode) {
    case SCROLL_LOCK_TG_CYR:
        if (record->event.pressed) {
            SEND_STRING(SS_TAP(X_SCROLLLOCK));
            layer_invert(1);
        }
        break;
    case TD(TD_AT_CTRL) ... TD(TD_BSPC_C_BSPC):
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
    [TD_T_RALT_QUOT]    = ACTION_TAP_DANCE_TAP_HOLD(KC_T, RALT(KC_QUOT)),
    [TD_T_RALT_T]       = ACTION_TAP_DANCE_TAP_HOLD(KC_T, RALT(KC_T)),
    [TD_U_RALT_U]       = ACTION_TAP_DANCE_TAP_HOLD(KC_U, RALT(KC_U)),
    [TD_P_LBRC]         = ACTION_TAP_DANCE_TAP_HOLD(KC_P, KC_LBRC),
    [TD_RALT_S_S]       = ACTION_TAP_DANCE_TAP_HOLD(RALT(KC_S), KC_S),
    [TD_RALT_QUOT_QUOT] = ACTION_TAP_DANCE_TAP_HOLD(RALT(KC_QUOT), KC_QUOT),
    [TD_M_RBRC]         = ACTION_TAP_DANCE_TAP_HOLD(KC_M, KC_RBRC),
    [TD_EQL_PLUS]       = ACTION_TAP_DANCE_TAP_HOLD(KC_EQL, KC_PLUS),
    [TD_GRAVE_TILDE]    = ACTION_TAP_DANCE_TAP_HOLD(KC_GRAVE, KC_TILDE),
    [TD_SLASH_QUES]     = ACTION_TAP_DANCE_TAP_HOLD(KC_SLASH, KC_QUES),
    [TD_BSLS_PIPE]      = ACTION_TAP_DANCE_TAP_HOLD(KC_BSLS, KC_PIPE),
    [TD_BSPC_C_BSPC]    = ACTION_TAP_DANCE_TAP_HOLD(KC_BSPC, LCTL(KC_BSPC))
};


#define DRK_LAYER 0
#define CYR_LAYER 1
#define NAV_LAYER 2
#define QWE_LAYER 3
#define SYM_LAYER 4
#define FNK_LAYER 5
#define STN_LAYER 6

const uint16_t PROGMEM combo_comm_dot[] = {KC_COMM, KC_DOT, COMBO_END};
const uint16_t PROGMEM combo_p_y[] = {KC_P, KC_Y, COMBO_END};
const uint16_t PROGMEM combo_u_h[] = {LSFT_T(KC_U), LSFT_T(KC_H), COMBO_END};
const uint16_t PROGMEM combo_f_g[] = {KC_F, KC_G, COMBO_END};
const uint16_t PROGMEM combo_c_r[] = {KC_C, KC_R, COMBO_END};
combo_t key_combos[COMBO_COUNT] = {
	COMBO(combo_comm_dot, KC_ESC),
    COMBO(combo_p_y, SCROLL_LOCK_TG_CYR),
    COMBO(combo_u_h, CAPSWRD),
    COMBO(combo_f_g, TG(STN_LAYER)),
    COMBO(combo_c_r, TG(QWE_LAYER))
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DRK_LAYER] = LAYOUT_split_3x6_3
    (KC_NO, KC_QUOT, KC_COMM, KC_DOT, KC_P, KC_Y,
     KC_F,  KC_G,    KC_C,    KC_R,   KC_L, KC_NO,

     KC_NO, LGUI_T(KC_A), LCTL_T(KC_O), LALT_T(KC_E), LSFT_T(KC_U), KC_I,
     KC_D,  LSFT_T(KC_H), LALT_T(KC_T), LCTL_T(KC_N), LGUI_T(KC_S), KC_NO,

     KC_NO, KC_SCLN, KC_Q, KC_J, KC_K, KC_X,
     KC_B,  KC_M,    KC_W, KC_V, KC_Z, KC_NO,

     KC_NO,                 LT(FNK_LAYER, KC_SPC), TD(TD_BSPC_C_BSPC),
     LT(NAV_LAYER, KC_TAB), LT(SYM_LAYER, KC_ENT), KC_NO
     ),

    [CYR_LAYER] = LAYOUT_split_3x6_3
    (KC_NO, TD(TD_Q_GRAVE),  TD(TD_W_QUES), TD(TD_E_SLASH), KC_R,          TD(TD_T_RALT_QUOT),
     KC_Y,  TD(TD_U_RALT_U), KC_I,          KC_O,           TD(TD_P_LBRC), KC_NO,

     KC_NO, LGUI_T(KC_A), RALT(KC_S),   LALT_T(KC_D), LSFT_T(KC_F),    KC_G,
     KC_H,  LSFT_T(KC_J), LALT_T(KC_K), LCTL_T(KC_L), LGUI_T(KC_SCLN), KC_NO,

     KC_NO, KC_Z, KC_X,    KC_C,   KC_V,          KC_B,
     KC_N,  KC_M, KC_COMM, KC_DOT, RALT(KC_RBRC), KC_TRNS,

     KC_TRNS, KC_TRNS, KC_TRNS,
     KC_TRNS, KC_TRNS, KC_TRNS
     ),

    [NAV_LAYER] = LAYOUT_split_3x6_3
    (KC_NO, KC_NO, KC_WH_D, KC_MS_U, KC_WH_U, KC_NO,      KC_NO, KC_VOLD, KC_UP,   KC_VOLU, KC_NO, KC_NO,
     KC_NO, KC_NO, KC_MS_L, KC_MS_D, KC_MS_R, KC_MS_BTN3, KC_NO, KC_LEFT, KC_DOWN, KC_RGHT, KC_NO, KC_NO,
     KC_NO, KC_NO, KC_NO,   KC_HOME, KC_END,  KC_NO,      KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO, KC_NO,

     KC_NO, KC_MS_BTN1, KC_MS_BTN2,
     KC_NO, KC_NO,      KC_NO
     ),

    [QWE_LAYER] = LAYOUT_split_3x6_3
    (KC_LSFT,  KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I,    KC_O,   KC_P,    KC_LBRC,
     KC_TAB,   KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K,    KC_L,   KC_SCLN, KC_QUOT,
     KC_LCTRL, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_RBRC, KC_TRNS,
     KC_LALT,  KC_SPC,  LT(FNK_LAYER, KC_BSPC),
     KC_TRNS,  KC_TRNS, KC_TRNS
     ),

    /* TODO: organize those better,
       right now the symbols just follow the numbers
    */
    [SYM_LAYER] = LAYOUT_split_3x6_3
    (KC_NO, KC_1, KC_2, KC_3, KC_4, KC_5,
     KC_6,  KC_7, KC_8, KC_9, KC_0, KC_NO,

     /* TODO: gui and shift */
     KC_NO,   KC_EXLM, TD(TD_AT_CTRL),  TD(TD_HASH_ALT),    KC_DLR,  KC_PERC,
     KC_CIRC, KC_AMPR, TD(TD_ASTR_ALT), TD(TD_LPAREN_CTRL), KC_RPRN, KC_NO,

     /* TODO: remove dances */
     KC_NO,            TD(TD_EQL_PLUS), TD(TD_GRAVE_TILDE), TD(TD_SLASH_QUES), KC_MINS, KC_UNDS,
     TD(TD_BSLS_PIPE), KC_LCBR,         KC_RCBR,            KC_LBRC,           KC_RBRC, KC_NO,

     KC_TRNS, KC_TRNS, KC_DEL,
     KC_TRNS, KC_TRNS, KC_TRNS
     ),

    [FNK_LAYER] = LAYOUT_split_3x6_3
    (KC_NO, KC_NO, KC_7,  KC_8,  KC_9,   KC_NO,
     KC_NO, KC_F7, KC_F8, KC_F9, KC_F10, KC_NO,

     KC_NO, KC_0,  KC_4,  KC_5,  KC_6,   KC_NO,
     KC_NO, KC_F4, KC_F5, KC_F6, KC_F11, KC_NO,

     KC_NO, KC_NO, KC_1,  KC_2,  KC_3,   KC_NO,
     KC_NO, KC_F1, KC_F2, KC_F3, KC_F12, KC_NO,

     KC_NO, KC_NO, KC_NO,
     KC_NO, KC_NO, KC_NO
     ),

    [STN_LAYER] = LAYOUT_split_3x6_3
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
