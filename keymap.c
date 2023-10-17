#include QMK_KEYBOARD_H
#include <stdio.h>
#include <keymap_ukrainian.h>
#include "features/adaptive_keys.h"
#include "features/tap_hold_dance.h"

#define ALPHA_LAYER 0
#define CYRILLIC_LAYER 1
#define CYRILLIC2_LAYER 2
#define NAVIGATION_LAYER1 3
#define NAVIGATION_LAYER2 4
#define GAMING_LAYER 5
#define SYMBOL1_LAYER 6
#define SYMBOL2_LAYER 7
#define NUMBER_LAYER 8
#define FN_LAYER 9

enum {
    TD_Q_GRAVE,
    TD_W_QUES,
    TD_E_SLASH,
    TD_T_QUOT,
    TD_U_BSLS,
    YOT_SOFT,
    HE_GE,
    KHA_EF,
    TD_P_LBRC
};

enum macros_keycodes {
    SCROLL_LOCK_TG_CYRILLIC = SAFE_RANGE,
    SCROLL_LOCK_TG_CYRILLIC2
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case OSM(MOD_LSFT):
        return 10000;
    default:
        return TAPPING_TERM;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_adaptive_key(keycode, record)) {
        return false;
    }
    tap_dance_action_t *action;

    switch (keycode) {
    case SCROLL_LOCK_TG_CYRILLIC:
        if (record->event.pressed) {
            tap_code(KC_SCROLL_LOCK);
            layer_invert(CYRILLIC_LAYER);
        }
        break;
    case SCROLL_LOCK_TG_CYRILLIC2:
        if (record->event.pressed) {
            tap_code(KC_SCROLL_LOCK);
            layer_invert(CYRILLIC2_LAYER);
        }
        break;
    case TD(TD_Q_GRAVE) ... TD(TD_P_LBRC):
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
    [TD_Q_GRAVE]     = ACTION_TAP_DANCE_TAP_HOLD(KC_Q, KC_GRAVE),
    [TD_W_QUES]      = ACTION_TAP_DANCE_TAP_HOLD(KC_W, KC_QUES),
    [TD_E_SLASH]     = ACTION_TAP_DANCE_TAP_HOLD(KC_E, KC_SLASH),
    [TD_T_QUOT]      = ACTION_TAP_DANCE_TAP_HOLD(KC_T, KC_QUOT),
    [TD_U_BSLS]      = ACTION_TAP_DANCE_TAP_HOLD(KC_U, KC_BSLS),
    [TD_P_LBRC]      = ACTION_TAP_DANCE_TAP_HOLD(KC_P, KC_LBRC),
    [YOT_SOFT]       = ACTION_TAP_DANCE_TAP_HOLD(UA_YOT, UA_SOFT),
    [HE_GE]          = ACTION_TAP_DANCE_TAP_HOLD(UA_HE, KC_BSLS),
    [KHA_EF]         = ACTION_TAP_DANCE_TAP_HOLD(UA_KHA, UA_EF),
};

enum combo_events {
    COMBO_ESC,
    COMBO_TAB,
    COMBO_ENTER,
    COMBO_CAPSWRD,
    COMBO_CYRILLIC,
    COMBO_CYRILLIC2,
    COMBO_GAMING,
    COMBO_NAVIGATION,
    COMBO_RALT,
    COMBO_PASTE,
    COMBO_TH,
    COMBO_CH,
    COMBO_SH,
    COMBO_GH,
    COMBO_PH,
    COMBO_WH,
    COMBO_J,
    COMBO_V,
    COMBO_Z,
    COMBO_QU,
    COMBO_Q,
    COMBO_LENGTH
};

// nt = th
const uint16_t PROGMEM combo_th[] = {LGUI_T(KC_T), LALT_T(KC_N), COMBO_END};
// FIXME: I don't like it at all
// nc = ch
const uint16_t PROGMEM combo_ch[] = {LALT_T(KC_N), LT(NUMBER_LAYER, KC_C), COMBO_END};
// sn = sh
const uint16_t PROGMEM combo_sh[] = {LCTL_T(KC_S), LALT_T(KC_N), COMBO_END};
// gm = gh
const uint16_t PROGMEM combo_gh[] = {KC_G, KC_M, COMBO_END};
// pm = ph
const uint16_t PROGMEM combo_ph[] = {KC_P, KC_M, COMBO_END};
// wd = wh
const uint16_t PROGMEM combo_wh[] = {KC_W, LT(NAVIGATION_LAYER1, KC_D), COMBO_END};

// fd = z
const uint16_t PROGMEM combo_z[] = {LT(SYMBOL1_LAYER, KC_F), LT(NAVIGATION_LAYER1, KC_D), COMBO_END};
// xf = v
const uint16_t PROGMEM combo_v[] = {KC_X, LT(SYMBOL1_LAYER, KC_F), COMBO_END};
// fl = j
const uint16_t PROGMEM combo_j[] = {LT(SYMBOL1_LAYER, KC_F), LT(SYMBOL2_LAYER, KC_L), COMBO_END};
// fld = q
const uint16_t PROGMEM combo_q[] = {LT(SYMBOL1_LAYER, KC_F), LT(SYMBOL2_LAYER, KC_L), LT(NAVIGATION_LAYER1, KC_D), COMBO_END};
// ld = qu
const uint16_t PROGMEM combo_qu[] = {LT(SYMBOL2_LAYER, KC_L), LT(NAVIGATION_LAYER1, KC_D), COMBO_END};

const uint16_t PROGMEM combo_esc[] = {LT(NUMBER_LAYER, KC_C), LGUI_T(KC_T), COMBO_END};
const uint16_t PROGMEM combo_tab[] = {LT(NUMBER_LAYER, KC_C), LCTL_T(KC_S), COMBO_END};
const uint16_t PROGMEM combo_enter[] = {LT(SYMBOL1_LAYER, KC_Y), KC_B, COMBO_END};
const uint16_t PROGMEM combo_capswrd[] = {OSM(MOD_LSFT), KC_BSPC, COMBO_END};
const uint16_t PROGMEM combo_cyrillic[] = {KC_P, KC_K, COMBO_END};
const uint16_t PROGMEM combo_cyrillic2[] = {KC_P, KC_W, COMBO_END};
const uint16_t PROGMEM combo_ralt[] = {LALT_T(KC_N), LALT_T(KC_E), COMBO_END};
const uint16_t PROGMEM combo_gaming[] = {LT(NAVIGATION_LAYER2, KC_U), LALT_T(KC_E), LT(SYMBOL1_LAYER, KC_Y), COMBO_END};
const uint16_t PROGMEM combo_navigation[] = {LT(SYMBOL1_LAYER, KC_F), LALT_T(KC_N), LT(NAVIGATION_LAYER1, KC_D), COMBO_END};

const uint16_t PROGMEM combo_paste[] = {KC_X, LT(SYMBOL1_LAYER, KC_F), LT(SYMBOL2_LAYER, KC_L), LT(NAVIGATION_LAYER1, KC_D), COMBO_END};

combo_t key_combos[] = {
    /* Missing important control keys */
	[COMBO_ESC] = COMBO(combo_esc, KC_ESC),
    [COMBO_TAB] = COMBO(combo_tab, KC_TAB),
    [COMBO_ENTER] = COMBO(combo_enter, KC_ENT),
    [COMBO_CAPSWRD] = COMBO(combo_capswrd, CW_TOGG),
    [COMBO_RALT] = COMBO(combo_ralt, KC_RALT),
    /* Useful shortcuts */
    [COMBO_PASTE] = COMBO(combo_paste, LCTL(KC_V)),
    /* Layer-switching combos */
    [COMBO_CYRILLIC] = COMBO(combo_cyrillic, SCROLL_LOCK_TG_CYRILLIC),
    [COMBO_CYRILLIC2] = COMBO(combo_cyrillic2, SCROLL_LOCK_TG_CYRILLIC2),
    [COMBO_GAMING] = COMBO(combo_gaming, TG(GAMING_LAYER)),
    [COMBO_NAVIGATION] = COMBO(combo_navigation, TG(NAVIGATION_LAYER2)),
    /* Alpha + h combos */
    [COMBO_TH] = COMBO_ACTION(combo_th),
    [COMBO_CH] = COMBO_ACTION(combo_ch),
    [COMBO_SH] = COMBO_ACTION(combo_sh),
    [COMBO_GH] = COMBO_ACTION(combo_gh),
    [COMBO_PH] = COMBO_ACTION(combo_ph),
    [COMBO_WH] = COMBO_ACTION(combo_wh),
    /* Missing keys */
    [COMBO_J] = COMBO(combo_j, KC_J),
    [COMBO_V] = COMBO(combo_v, KC_V),
    [COMBO_Z] = COMBO(combo_z, KC_Z),
    [COMBO_Q] = COMBO(combo_q, KC_Q),
    [COMBO_QU] = COMBO_ACTION(combo_qu),
};
uint16_t COMBO_LEN = COMBO_LENGTH;

void process_combo_event(uint16_t combo_index, bool pressed) {
	switch (combo_index) {
	case COMBO_TH:
		if (pressed) {
			set_capsword_press(KC_T);
            set_capsword_press(KC_H);
		}
		break;
	case COMBO_CH:
		if (pressed) {
            set_capsword_press(KC_C);
            set_capsword_press(KC_H);
		}
		break;
	case COMBO_SH:
		if (pressed) {
            set_capsword_press(KC_S);
            set_capsword_press(KC_H);
		}
		break;
	case COMBO_GH:
		if (pressed) {
            set_capsword_press(KC_G);
            set_capsword_press(KC_H);
		}
		break;
	case COMBO_PH:
		if (pressed) {
            set_capsword_press(KC_P);
            set_capsword_press(KC_H);
		}
		break;
    case COMBO_WH:
        if (pressed) {
            set_capsword_press(KC_W);
            set_capsword_press(KC_H);
        }
        break;
    case COMBO_QU:
        if (pressed) {
            set_capsword_press(KC_Q);
            set_capsword_press(KC_U);
        }
        break;
	}
}

bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    if (layer_state_is(NAVIGATION_LAYER2) && combo_index != COMBO_NAVIGATION) {
        return false;
    }

    if (layer_state_is(SYMBOL1_LAYER) || layer_state_is(SYMBOL2_LAYER)) {
        return false;
    }

    switch (combo_index) {
    case COMBO_CYRILLIC:
        if (!(layer_state_is(ALPHA_LAYER) || layer_state_is(CYRILLIC_LAYER))) {
            return false;
        }
        break;
    case COMBO_CYRILLIC2:
        if (!(layer_state_is(ALPHA_LAYER) || layer_state_is(CYRILLIC2_LAYER))) {
            return false;
        }
        break;
    case COMBO_GAMING:
        if (!(layer_state_is(ALPHA_LAYER) || layer_state_is(GAMING_LAYER))) {
            return false;
        }
        break;
    case COMBO_TH:
    case COMBO_CH:
    case COMBO_SH:
    case COMBO_GH:
    case COMBO_PH:
    case COMBO_WH:
    case COMBO_QU:
    case COMBO_J:
    case COMBO_V:
    case COMBO_Z:
    case COMBO_Q:
        if (!layer_state_is(ALPHA_LAYER)) {
            return false;
        }
        break;

    }

    return true;
}

const key_override_t dot_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_DOT, S(KC_SCLN));
const key_override_t comm_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_COMM, KC_SCLN);

const key_override_t **key_overrides = (const key_override_t *[]){
    &dot_key_override,
    &comm_key_override,
    NULL
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [ALPHA_LAYER] = LAYOUT_split_3x6_3
    (KC_NO, KC_NO,   KC_G,    KC_M,     KC_P,  KC_NO,
     KC_NO, KC_MINS, KC_QUOT, KC_SLASH, KC_NO, KC_NO,

     KC_NO,   LT(NUMBER_LAYER, KC_C), LCTL_T(KC_S), LALT_T(KC_N), LGUI_T(KC_T),           KC_K,
     KC_COMM, LGUI_T(KC_A),           LALT_T(KC_E), LCTL_T(KC_I), LT(NUMBER_LAYER, KC_H), KC_NO,

     KC_NO,  KC_X,                        LT(SYMBOL1_LAYER, KC_F), LT(SYMBOL2_LAYER, KC_L), LT(NAVIGATION_LAYER1, KC_D), KC_W,
     KC_DOT, LT(NAVIGATION_LAYER2, KC_U), LT(SYMBOL2_LAYER, KC_O), LT(SYMBOL1_LAYER, KC_Y), KC_B,                       KC_NO,

     KC_NO,         KC_R,   KC_BSPC,
     OSM(MOD_LSFT), KC_SPC, KC_NO
     ),

    [CYRILLIC_LAYER] = LAYOUT_split_3x6_3
    (KC_NO, KC_Q,           KC_W, KC_E, KC_R,          TD(TD_T_QUOT),
     KC_Y,  TD(TD_U_BSLS),  KC_I, KC_O, TD(TD_P_LBRC), KC_NO,

     KC_NO, LT(NUMBER_LAYER, KC_A), LCTL_T(KC_S), LALT_T(KC_D), LGUI_T(KC_F),              KC_G,
     KC_H,  LGUI_T(KC_J),           LALT_T(KC_K), LCTL_T(KC_L), LT(NUMBER_LAYER, KC_SCLN), KC_NO,

     KC_NO, KC_Z,                        LT(SYMBOL1_LAYER, KC_X),    LT(SYMBOL2_LAYER, KC_C),   LT(NAVIGATION_LAYER1, KC_V), KC_B,
     KC_N,  LT(NAVIGATION_LAYER2, KC_M), LT(SYMBOL2_LAYER, KC_COMM), LT(SYMBOL1_LAYER, KC_DOT), KC_RBRC,                    KC_NO,

     KC_TRNS, KC_TRNS, KC_TRNS,
     KC_TRNS, KC_TRNS, KC_TRNS
     ),

    [CYRILLIC2_LAYER] = LAYOUT_split_3x6_3
    (KC_NO, UA_ZHE, TD(YOT_SOFT), UA_KA, TD(KHA_EF), UA_TSE, UA_SHCH, UA_YE, UA_A, UA_U, UA_YU, KC_NO,

     KC_NO, LGUI_T(UA_ES), LCTL_T(UA_VE), LALT_T(UA_EN), UA_TE, UA_EM,
     UA_SHA, UA_ER, LALT_T(UA_O), LCTL_T(UA_I), LGUI_T(UA_Y), KC_NO,

     KC_NO, LT(NUMBER_LAYER, UA_ZE), LT(SYMBOL1_LAYER, UA_BE), LT(SYMBOL2_LAYER, UA_PE), LT(NAVIGATION_LAYER1, UA_DE), TD(HE_GE),
     UA_CHE, LT(NAVIGATION_LAYER2, UA_EL), LT(SYMBOL2_LAYER, UA_YA), LT(SYMBOL1_LAYER, UA_E), LT(NUMBER_LAYER, UA_YI), KC_NO,

     KC_TRNS, KC_TRNS, KC_TRNS,
     KC_TRNS, KC_TRNS, KC_TRNS
     ),

    [NAVIGATION_LAYER1] = LAYOUT_split_3x6_3
    (KC_NO, KC_NO, KC_NO,   KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO, KC_NO,
     KC_NO, KC_NO, KC_LCTL, KC_LALT, KC_NO, KC_NO, KC_NO, KC_VOLD, KC_UP,   KC_VOLU, KC_NO, KC_NO,
     KC_NO, KC_NO, KC_NO,   KC_NO,   KC_NO, KC_NO, KC_NO, KC_LEFT, KC_DOWN, KC_RGHT, KC_NO, KC_NO,

     KC_NO,   KC_MS_BTN1, KC_MS_BTN2,
     KC_PGDN, KC_PGUP,    KC_NO
     ),

    [NAVIGATION_LAYER2] = LAYOUT_split_3x6_3
    (KC_NO, KC_NO,      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO, KC_NO,
     KC_NO, KC_NO,      KC_WH_D, KC_MS_U, KC_WH_U, KC_HOME, KC_NO, KC_VOLD, KC_UP,   KC_VOLU, KC_NO, KC_NO,
     KC_NO, KC_MS_BTN3, KC_MS_L, KC_MS_D, KC_MS_R, KC_END,  KC_NO, KC_LEFT, KC_DOWN, KC_RGHT, KC_NO, KC_NO,

     KC_NO,   KC_MS_BTN1, KC_MS_BTN2,
     KC_PGDN, KC_PGUP,    KC_NO
     ),

    [GAMING_LAYER] = LAYOUT_split_3x6_3
    (KC_NO, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I,    KC_O,   KC_P,    KC_NO,
     KC_NO, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K,    KC_L,   LT(NUMBER_LAYER, KC_SCLN), KC_NO,
     KC_NO, LCTL_T(KC_Z), LALT_T(KC_X), KC_C, KC_V, LT(NUMBER_LAYER, KC_B), KC_N, KC_M, KC_COMM, KC_DOT, KC_RBRC, KC_NO,
     KC_NO,   KC_SPC,  KC_LSFT,
     KC_TRNS, KC_TRNS, KC_NO
     ),

    [SYMBOL1_LAYER] = LAYOUT_split_3x6_3
    (KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,

     KC_NO, KC_TILDE, KC_COMM,  KC_DOT, KC_SLASH, KC_NO, KC_NO, S(KC_MINS), KC_SCLN, KC_AMPR,    KC_QUES, KC_NO,
     KC_NO, KC_DLR,   KC_GRAVE, KC_AT,  KC_BSLS,  KC_NO, KC_NO, S(KC_QUOT), KC_QUOT, S(KC_BSLS), KC_CIRC, KC_NO,

     KC_TRNS, KC_TRNS, KC_TRNS,
     KC_TRNS, KC_TRNS, KC_TRNS
     ),

    [SYMBOL2_LAYER] = LAYOUT_split_3x6_3
    (KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,

     KC_NO, KC_LT,   KC_LPRN, KC_RPRN, KC_GT,   KC_NO, KC_NO, KC_MINS, KC_EQL,  KC_HASH, KC_EXLM,    KC_NO,
     KC_NO, KC_LBRC, KC_LCBR, KC_RCBR, KC_RBRC, KC_NO, KC_NO, KC_PLUS, KC_PERC, KC_ASTR, S(KC_SCLN), KC_NO,

     KC_TRNS, KC_TRNS, KC_TRNS,
     KC_TRNS, KC_TRNS, KC_TRNS
     ),

    [NUMBER_LAYER] = LAYOUT_split_3x6_3
    (KC_NO, KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO,
     /*     it's -                            */
     KC_NO, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO,

     KC_NO, KC_8, KC_7, KC_6, KC_5, KC_NO, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_NO,
     KC_NO, KC_4, KC_3, KC_2, KC_1, KC_NO, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,  KC_NO,

     KC_TRNS, KC_9, KC_0, KC_F11, KC_F12, KC_TRNS
     )
};
