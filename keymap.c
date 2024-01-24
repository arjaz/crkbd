#include QMK_KEYBOARD_H
#include <stdio.h>
#include <keymap_ukrainian.h>
#include <keymap_steno.h>
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
#define STENO_LAYER 10

#define AR_J KC_J
#define AR_G KC_G
#define AR_M KC_M
#define AR_P KC_P
#define AR_V KC_V
#define AR_ASTR KC_ASTR
#define AR_MINS KC_MINS
#define AR_QUOT KC_QUOT
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
#define AR_F LT(SYMBOL1_LAYER, KC_F)
#define AR_L LT(SYMBOL2_LAYER, KC_L)
#define AR_D LT(NAVIGATION_LAYER1, KC_D)
#define AR_W KC_W
#define AR_DOT KC_DOT
#define AR_U LT(NAVIGATION_LAYER2, KC_U)
#define AR_O LT(SYMBOL2_LAYER, KC_O)
#define AR_Y LT(SYMBOL1_LAYER, KC_Y)
#define AR_B KC_B
#define AR_R LT(NUMBER_LAYER, KC_R)
#define AR_BSPC KC_BSPC
#define AR_LSFT OSM(MOD_LSFT)
#define AR_SPC LT(NUMBER_LAYER, KC_SPC)

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
    COMBO_STENO,
    COMBO_GAMING,
    COMBO_NAVIGATION,
    COMBO_RALT,
    COMBO_COPY,
    COMBO_PASTE,
    COMBO_TH,
    COMBO_CH,
    COMBO_SH,
    COMBO_GH,
    COMBO_PH,
    COMBO_WH,
    COMBO_Z,
    COMBO_Q,
    COMBO_SLASH,
    COMBO_LENGTH
};

// nt = th
const uint16_t PROGMEM combo_th[] = {AR_T, AR_N, COMBO_END};
// nc = ch
const uint16_t PROGMEM combo_ch[] = {AR_N, AR_C, COMBO_END};
// sn = sh
const uint16_t PROGMEM combo_sh[] = {AR_S, AR_N, COMBO_END};
// gm = gh
const uint16_t PROGMEM combo_gh[] = {AR_G, AR_M, COMBO_END};
// pm = ph
const uint16_t PROGMEM combo_ph[] = {AR_P, AR_M, COMBO_END};
// wd = wh
const uint16_t PROGMEM combo_wh[] = {AR_W, AR_D, COMBO_END};

// fd = z
const uint16_t PROGMEM combo_z[] = {AR_F, AR_D, COMBO_END};
// ld = q
const uint16_t PROGMEM combo_q[] = {AR_L, AR_D, COMBO_END};

// =' = /
const uint16_t PROGMEM combo_slash[] = {AR_QUOT, AR_EQL, COMBO_END};

const uint16_t PROGMEM combo_esc[] = {AR_C, AR_T, COMBO_END};
const uint16_t PROGMEM combo_tab[] = {AR_C, AR_S, COMBO_END};
const uint16_t PROGMEM combo_enter[] = {AR_Y, AR_B, COMBO_END};
const uint16_t PROGMEM combo_ralt[] = {AR_N, AR_E, COMBO_END};

const uint16_t PROGMEM combo_capswrd[] = {AR_LSFT, AR_BSPC, COMBO_END};
const uint16_t PROGMEM combo_cyrillic[] = {AR_P, AR_K, COMBO_END};
const uint16_t PROGMEM combo_cyrillic2[] = {AR_P, AR_W, COMBO_END};
const uint16_t PROGMEM combo_steno[] = {AR_MINS, AR_COMM, COMBO_END};
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
    [COMBO_CYRILLIC2] = COMBO(combo_cyrillic2, SCROLL_LOCK_TG_CYRILLIC2),
    [COMBO_STENO] = COMBO(combo_steno, TG(STENO_LAYER)),
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
    [COMBO_Z] = COMBO(combo_z, KC_Z),
    [COMBO_Q] = COMBO(combo_q, KC_Q),
    [COMBO_SLASH] = COMBO(combo_slash, KC_SLASH),
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
	}
}

bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    if (layer_state_is(STENO_LAYER) && combo_index != COMBO_STENO) {
        return false;
    }

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
const key_override_t slash_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_SLASH, KC_BSLS);

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
    (KC_Q, KC_W,           KC_E, KC_R, TD(TD_T_QUOT),
     KC_Y, TD(TD_U_BSLS),  KC_I, KC_O, TD(TD_P_LBRC),

     KC_A, LCTL_T(KC_S), LALT_T(KC_D), LGUI_T(KC_F), KC_G,
     KC_H, LGUI_T(KC_J), LALT_T(KC_K), LCTL_T(KC_L), KC_SCLN,

     KC_Z, LT(SYMBOL1_LAYER, KC_X),     LT(SYMBOL2_LAYER, KC_C),    LT(NAVIGATION_LAYER1, KC_V), KC_B,
     KC_N, LT(NAVIGATION_LAYER2, KC_M), LT(SYMBOL2_LAYER, KC_COMM), LT(SYMBOL1_LAYER, KC_DOT),   KC_RBRC,

     KC_TRNS, KC_TRNS, KC_TRNS,
     KC_TRNS, KC_TRNS, KC_TRNS
     ),

    [CYRILLIC2_LAYER] = LAYOUT_split_3x5_3
    (UA_ZHE, TD(YOT_SOFT), UA_KA, TD(KHA_EF), UA_TSE, UA_SHCH, UA_YE, UA_A, UA_U, UA_YU,

     LGUI_T(UA_ES), LCTL_T(UA_VE), LALT_T(UA_EN), UA_TE, UA_EM,
     UA_SHA, UA_ER, LALT_T(UA_O), LCTL_T(UA_I), LGUI_T(UA_Y),

     LT(NUMBER_LAYER, UA_ZE), LT(SYMBOL1_LAYER, UA_BE), LT(SYMBOL2_LAYER, UA_PE), LT(NAVIGATION_LAYER1, UA_DE), TD(HE_GE),
     UA_CHE, LT(NAVIGATION_LAYER2, UA_EL), LT(SYMBOL2_LAYER, UA_YA), LT(SYMBOL1_LAYER, UA_E), LT(NUMBER_LAYER, UA_YI),

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

    [SYMBOL1_LAYER] = LAYOUT_split_3x5_3
    (KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,

     KC_TILDE, KC_COMM,  KC_DOT, KC_SLASH, KC_TRNS, KC_TRNS, S(KC_MINS), KC_SCLN, KC_AMPR,    KC_QUES,
     KC_DLR,   KC_GRAVE, KC_AT,  KC_BSLS,  KC_TRNS, KC_TRNS, S(KC_QUOT), KC_QUOT, S(KC_BSLS), KC_CIRC,

     KC_TRNS, KC_TRNS, KC_TRNS,
     KC_TRNS, KC_TRNS, KC_TRNS
     ),

    [SYMBOL2_LAYER] = LAYOUT_split_3x5_3
    (KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,

     KC_LT,   KC_LPRN, KC_RPRN, KC_GT,   KC_TRNS, KC_TRNS, KC_MINS, KC_EQL,  KC_HASH, KC_EXLM,
     KC_LBRC, KC_LCBR, KC_RCBR, KC_RBRC, KC_TRNS, KC_TRNS, KC_PLUS, KC_PERC, KC_ASTR, S(KC_SCLN),

     KC_TRNS, KC_PERC, KC_SLASH,
     KC_TRNS, KC_TRNS, KC_TRNS
     ),

    [NUMBER_LAYER] = LAYOUT_split_3x5_3
    (KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_F9, KC_F10, KC_F11, KC_F12,
     KC_8,  KC_7,  KC_6,  KC_5,  KC_NO, KC_TRNS, KC_F5, KC_F6,  KC_F7,  KC_F8,
     KC_4,  KC_3,  KC_2,  KC_1,  KC_NO, KC_TRNS, KC_F1, KC_F2,  KC_F3,  KC_F4,

     KC_TRNS, KC_9, KC_0, KC_TRNS, KC_TRNS, KC_TRNS
     ),

    [STENO_LAYER] = LAYOUT_split_3x5_3
    (STN_N1, STN_N2, STN_N3, STN_N4, STN_N5,
     STN_N6, STN_N7, STN_N8, STN_N9, STN_N9,

     STN_S1, STN_TL, STN_PL, STN_HL, STN_ST1,
     STN_FR, STN_PR, STN_LR, STN_TR, STN_DR,

     STN_S2, STN_KL, STN_WL, STN_RL, STN_ST2,
     STN_RR, STN_BR, STN_GR, STN_SR, STN_ZR,

     KC_NO, STN_A, STN_O,
     STN_E, STN_U, KC_NO
     )
};
