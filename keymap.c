#include QMK_KEYBOARD_H
#include <stdio.h>

/* void on_dance_at(qk_tap_dance_state_t *state, void *user_data) { */
/*     if(state->count == 3) { */
/*         tap_code16(KC_AT); */
/*         tap_code16(KC_AT); */
/*         tap_code16(KC_AT); */
/*     } */
/*     if(state->count > 3) { */
/*         tap_code16(KC_AT); */
/*     } */
/* } */

/* void dance_at_finished(qk_tap_dance_state_t *state, void *user_data) { */
/*     dance_state1[9].step = dance_step(state); */
/*     switch (dance_state[9].step) { */
/*         case SINGLE_TAP: register_code16(KC_AT); break; */
/*         case SINGLE_HOLD: register_code16(KC_LCTRL); break; */
/*         case DOUBLE_TAP: register_code16(KC_AT); register_code16(KC_AT); break; */
/*         case DOUBLE_SINGLE_TAP: tap_code16(KC_AT); register_code16(KC_AT); */
/*     } */
/* } */

/* void dance_at_reset(qk_tap_dance_state_t *state, void *user_data) { */
/*     wait_ms(10); */
/*     switch (dance_state[9].step) { */
/*         case SINGLE_TAP: unregister_code16(KC_AT); break; */
/*         case SINGLE_HOLD: unregister_code16(KC_LCTRL); break; */
/*         case DOUBLE_TAP: unregister_code16(KC_AT); break; */
/*         case DOUBLE_SINGLE_TAP: unregister_code16(KC_AT); break; */
/*     } */
/*     dance_state[9].step = 0; */
/* } */

/* enum { */
/*     TD_AT_CTRL; */
/* }; */

/* // Tap Dance definitions */
/* qk_tap_dance_action_t tap_dance_actions[] = { */
/*     // Tap once for Escape, twice for Caps Lock */
/*     [TD_AT_CTRL] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_at, dance_at_finished, dance_at_reset), */
/* }; */

/* TODO: tap-dance cyrillics @, #, * and ( */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base dvorak layer */
    [0] = LAYOUT_split_3x6_3
    (KC_SCRL, KC_QUOT, KC_COMM,      KC_DOT,       KC_P, KC_Y, KC_F, KC_G, KC_C,         KC_R,         KC_L, KC_NO,
     KC_TAB,  KC_A,    LCTL_T(KC_O), LALT_T(KC_E), KC_U, KC_I, KC_D, KC_H, LALT_T(KC_T), LCTL_T(KC_N), KC_S, KC_NO,
     TG(1),   KC_SCLN, KC_Q,         KC_J,         KC_K, KC_X, KC_B, KC_M, KC_W,         KC_V,         KC_Z, OSM(MOD_RALT),

     LT(5, KC_BSPC), KC_SPC,         LGUI_T(KC_ESC),
     MO(4),          LSFT_T(KC_ENT), MO(2)
     ),

    /* TODO: Cyrillic layer with tap-dance for alternative letters */
    [1] = LAYOUT_split_3x6_3
    (KC_TRNS, KC_Q, KC_W,       KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I,    KC_O,   KC_P,          KC_LBRC,
     KC_TRNS, KC_A, RALT(KC_S), KC_D, KC_F, KC_G, KC_H, KC_J, KC_K,    KC_L,   KC_SCLN,       RALT(KC_QUOT),
     TG(1),   KC_Z, KC_X,       KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, RALT(KC_RBRC), KC_TRNS,

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
    [3] = LAYOUT_split_3x6_3
    (KC_LSFT, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I,    KC_O,   KC_P,    KC_LBRC,
     KC_TAB,  KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K,    KC_L,   KC_SCLN, KC_QUOT,
     TO(0),   KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_RBRC, KC_TRNS,
     KC_TRNS, KC_TRNS, KC_TRNS,
     KC_TRNS, KC_TRNS, KC_TRNS
     ),

    /* TODO: Symbols layer with tap-dance */
    [4] = LAYOUT_split_3x6_3
    (KC_NO, KC_1, KC_2, KC_3, KC_4, KC_5,
     KC_6,  KC_7, KC_8, KC_9, KC_0, KC_NO,

     /* I can't use that with mod-taps as the keys here are shifted, I have to use tap-dances */
     KC_NO, KC_EXLM, KC_AT, KC_HASH, KC_DLR,  KC_PERC,
     KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_NO,

     TO(3),   KC_EQL,  KC_GRAVE, KC_SLSH, KC_MINS, KC_UNDS,
     KC_BSLS, KC_LCBR, KC_RCBR,  KC_LBRC, KC_RBRC, KC_NO,

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
