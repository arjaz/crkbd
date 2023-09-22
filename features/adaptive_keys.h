#pragma once

static uint8_t prior_saved_mods = 0;
static uint16_t prior_keycode = KC_NO;
static uint32_t prior_keydown = 0;

void set_capsword_press(uint16_t keycode) {
    if (is_caps_word_on() && !caps_word_press_user(keycode)) {
        add_weak_mods(MOD_MASK_SHIFT);
    }
    tap_code(keycode);
}

bool process_adaptive_key(uint16_t keycode, const keyrecord_t *record) {
#ifdef ADAPTIVE_ONLY_FROM_LAYER
	if (!(layer_state_is(ADAPTIVE_ONLY_FROM_LAYER) || layer_state_is(ADAPTIVE_ONLY_FROM_LAYER + 1))) {
		return true;
	}
#endif
	bool return_state = true;

	if (record->event.pressed) {
		uint8_t saved_mods = get_mods();
		if (saved_mods & MOD_MASK_CAG) {
			// don't interfere if modifiers are involved
			return true;
		}
		if (timer_elapsed32(prior_keydown) < ADAPTIVE_TERM) {
			if (!is_caps_word_on()) {
				unregister_mods(MOD_MASK_SHIFT);
			}
			switch (keycode & 0xFF) {
			case KC_H:
				switch (prior_keycode) {
				case KC_A:
					set_capsword_press(KC_U);
					return_state = false;
					break;
				case KC_E:
				    set_capsword_press(KC_O);
				    return_state = false;
				    break;
				case KC_U:
					set_capsword_press(KC_A);
					return_state = false;
					break;
				case KC_O:
				    set_capsword_press(KC_E);
				    return_state = false;
				    break;
				}
				break;
			}

			if (return_state) {
				set_mods(saved_mods);
				prior_keycode = KC_NO;
				prior_keydown = 0;
				prior_saved_mods = 0;
			}
		}
		// Restore mods
		prior_saved_mods = saved_mods;
		// this keycode is stripped of mods+taps
		prior_keycode = keycode & 0xFF;
		// (re)start prior_key timing
		prior_keydown = timer_read32();
	}
	return return_state;
}
