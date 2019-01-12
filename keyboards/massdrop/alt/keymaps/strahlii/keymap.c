#include QMK_KEYBOARD_H

enum alt_keycodes {
    L_BRI = SAFE_RANGE, //LED Brightness Increase
    L_BRD,              //LED Brightness Decrease
    L_PTN,              //LED Pattern Select Next
    L_PTP,              //LED Pattern Select Previous
    L_PSI,              //LED Pattern Speed Increase
    L_PSD,              //LED Pattern Speed Decrease
    L_T_MD,             //LED Toggle Mode
    L_T_ONF,            //LED Toggle On / Off
    L_ON,               //LED On
    L_OFF,              //LED Off
    L_T_BR,             //LED Toggle Breath Effect
    L_T_PTD,            //LED Toggle Scrolling Pattern Direction
    U_T_AUTO,           //USB Extra Port Toggle Auto Detect / Always Active
    U_T_AGCR,           //USB Toggle Automatic GCR control
    DBG_TOG,            //DEBUG Toggle On / Off
    DBG_MTRX,           //DEBUG Toggle Matrix Prints
    DBG_KBD,            //DEBUG Toggle Keyboard Prints
    DBG_MOU,            //DEBUG Toggle Mouse Prints
    MD_BOOT,            //Restart into bootloader after hold timeout
	KX_UMLAUT_A,
	KX_UMLAUT_O,
	KX_UMLAUT_U,
	KX_UMLAUT_S,
	KX_EURO,
};

#define TG_NKRO MAGIC_TOGGLE_NKRO //Toggle 6KRO / NKRO mode
#define MODS_SHIFT_MASK  (MOD_BIT(KC_LSHIFT)|MOD_BIT(KC_RSHIFT)) //sends altkeycodes


bool g_bOsNumLockOn = false;

void led_set_user(uint8_t usb_led) {

	// Update saved numlock state

	if (usb_led & (1<<USB_LED_NUM_LOCK))
	{
		g_bOsNumLockOn = true;
	}
	else
	{
		g_bOsNumLockOn = false;
	}
}

void send_altcode(uint16_t mask, keyrecord_t *record) {
	
	/* Sends the "alt code" defined in mask, i.e. send_altcode(1234) 
	holds left alt while sending KP_1, KP_2, KP_3 and KP_4 taps, 
	then releases left alt if it wasn't being held by the user. 
	
	NOTE: If your alt code starts with a 0, leave that out, else the
	mask will be treated as octal and your firmware won't compile.
	send_altcode(123) will output KP_0, KP_1, KP_2 and KP_3. */
	
	// Check & save mod state
	
	static uint8_t lalt_mask;
	lalt_mask = keyboard_report->mods & KC_LALT;
	
	// Check & save numlock state
	
	bool bNumLockWasOn = g_bOsNumLockOn;
	
	// Split up the mask into its 4 decimals
	
	static uint16_t kp[4];
	
	kp[0] = mask / 1000;
	kp[1] = mask / 100 - kp[0] * 100;
	kp[2] = mask / 10 - kp[0] * 1000 - kp[1] * 10;
	kp[3] = mask - kp[0] * 1000 - kp[1] * 100 - kp[2] * 10;
	
	// Convert to keycodes
	
	for (uint8_t i=0; i<=3; i++) {
		switch(kp[i]) {
			case 0:
				kp[i] = KC_KP_0; break;
			case 1:
				kp[i] = KC_KP_1; break;
			case 2:
				kp[i] = KC_KP_2; break;
			case 3:
				kp[i] = KC_KP_3; break;
			case 4:
				kp[i] = KC_KP_4; break;
			case 5:
				kp[i] = KC_KP_5; break;
			case 6:
				kp[i] = KC_KP_6; break;
			case 7:
				kp[i] = KC_KP_7; break;
			case 8:
				kp[i] = KC_KP_8; break;
			case 9:
				kp[i] = KC_KP_9; break;
		}
	}
	
	// Put ALT into pressed state
	
	if (!lalt_mask) {
		register_code(KC_LALT);
		send_keyboard_report();
	}
	
	// If not enabled, enable numlock
	
	if (!bNumLockWasOn) {
			register_code(KC_LNUM);
			send_keyboard_report();
	}
	
	// The send_keyboard_report() spam is not pretty, but necessary.
	
	add_key(kp[0]);
	send_keyboard_report();
	del_key(kp[0]);
	send_keyboard_report();
	add_key(kp[1]);
	send_keyboard_report();		
	del_key(kp[1]);
	send_keyboard_report();
	add_key(kp[2]);
	send_keyboard_report();		
	del_key(kp[2]);
	send_keyboard_report();
	add_key(kp[3]);
	send_keyboard_report();
	del_key(kp[3]);
	send_keyboard_report();
	
	// If user wasn't pressing ALT, release it
	
	if (!lalt_mask) {
		unregister_code(KC_LALT);
	}
	
	send_keyboard_report();
	
	// If it wasn't enabled before, disable numlock
	
	if (!bNumLockWasOn) {
			unregister_code(KC_LNUM);
			send_keyboard_report();
	}
}



keymap_config_t keymap_config;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_CAPS,       KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,  \
        KC_TAB,        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_HOME, \
        LT(1, KC_ESC), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGUP, \
        KC_LSFT,       KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,   KC_PGDN, \
        KC_LCTL,       KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(2),   KC_LEFT, KC_DOWN, KC_RGHT  \
    ),
    [1] = LAYOUT(
        KC_GRV,  KC_F1,       KC_F2,       KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,       KC_F8,   KC_F9,       KC_F10,  KC_F11,  KC_F12,  KC_DEL,  KC_PAUS, \
        KC_TRNS, KC_WBAK,     KC_WFWD,     KX_EURO, KC_WREF, KC_TRNS, KC_TRNS, KX_UMLAUT_U, KC_TRNS, KX_UMLAUT_O, KC_FIND, KC_INS,  KC_PSCR, KC_SLCK, KC_END, \
        KC_TRNS, KX_UMLAUT_A, KX_UMLAUT_S, KC_TRNS, KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN,     KC_UP,   KC_RGHT,     KC_TRNS, KC_TRNS,          KC_TRNS, KC_VOLU, \
        KC_TRNS, KC_TRNS,     KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS,          KC_MPLY, KC_VOLD, \
        KC_TRNS, KC_TRNS,     KC_TRNS,                                KC_TRNS,                                    KC_TRNS, KC_TRNS, KC_MPRV, KC_MUTE, KC_MNXT  \
    ),
    [2] = LAYOUT(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_SLEP, KC_TRNS, \
        L_T_BR,  L_PSD,   L_BRI,   L_PSI,   KC_TRNS, KC_TRNS, KC_TRNS, U_T_AUTO,U_T_AGCR,KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, \
        L_T_PTD, L_PTP,   L_BRD,   L_PTN,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_BRIU, \
        KC_TRNS, L_T_MD,  L_T_ONF, KC_TRNS, KC_TRNS, MD_BOOT, TG_NKRO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_BRID, \
        KC_PWR,  KC_TRNS, KC_TRNS,                            KC_WAKE,                            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS  \
    ),
    /*
    [X] = LAYOUT(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, \
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, \
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, \
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, \
        KC_TRNS, KC_TRNS, KC_TRNS,                            KC_TRNS,                            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS  \
    ),
    */
};

const uint16_t PROGMEM fn_actions[] = {

};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
};

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
};

#define MODS_SHIFT  (keyboard_report->mods & MOD_BIT(KC_LSHIFT) || keyboard_report->mods & MOD_BIT(KC_RSHIFT))
#define MODS_CTRL  (keyboard_report->mods & MOD_BIT(KC_LCTL) || keyboard_report->mods & MOD_BIT(KC_RCTRL))
#define MODS_ALT  (keyboard_report->mods & MOD_BIT(KC_LALT) || keyboard_report->mods & MOD_BIT(KC_RALT))

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint32_t key_timer;
    static uint8_t shift_mask;
	shift_mask = get_mods()&MODS_SHIFT_MASK;


if (record->event.pressed) {
		switch(keycode) {
		case KX_UMLAUT_A:
			if (shift_mask) { // We want a capital version of the German A Umlaut 'cause shift is pressed
				unregister_code(KC_LSFT); // Gotta temporarily disable both shift keys, else this whole thing may behave oddly
				unregister_code(KC_RSFT);
				send_keyboard_report();
				
				send_altcode(196, record); // This is where the magic happens
				
				if (shift_mask &MOD_BIT(KC_LSFT)) register_code(KC_LSFT); // Restore shift keys to previous state
				if (shift_mask &MOD_BIT(KC_RSFT)) register_code(KC_RSFT);
				
				send_keyboard_report();
			} else {
				send_altcode(228, record); // ä
			}
			return false;
			break;
		case KX_UMLAUT_O:
			if (shift_mask) {
				unregister_code(KC_LSFT);
				unregister_code(KC_RSFT);
				send_keyboard_report();
				
				send_altcode(214, record); // Ö
				
				if (shift_mask &MOD_BIT(KC_LSFT)) register_code(KC_LSFT);
				if (shift_mask &MOD_BIT(KC_RSFT)) register_code(KC_RSFT);
				
				send_keyboard_report();
			} else {
				send_altcode(246, record); // ö
			}
			return false;
			break;
		case KX_UMLAUT_U:
			if (shift_mask) {
				unregister_code(KC_LSFT);
				unregister_code(KC_RSFT);
				send_keyboard_report();
				
				send_altcode(220, record); // Ü
				
				if (shift_mask &MOD_BIT(KC_LSFT)) register_code(KC_LSFT);
				if (shift_mask &MOD_BIT(KC_RSFT)) register_code(KC_RSFT);
				
				send_keyboard_report();
			} else {
				send_altcode(252, record); // ü
			}
			return false;
			break;
		case KX_UMLAUT_S:
			send_altcode(223, record); // ß
			return false;
			break;
		case KX_EURO:
			send_altcode(128, record); // ß
			return false;
			break;
		

        case L_BRI:
            if (record->event.pressed) {
                if (LED_GCR_STEP > LED_GCR_MAX - gcr_desired) gcr_desired = LED_GCR_MAX;
                else gcr_desired += LED_GCR_STEP;
                if (led_animation_breathing) gcr_breathe = gcr_desired;
            }
            return false;
        case L_BRD:
            if (record->event.pressed) {
                if (LED_GCR_STEP > gcr_desired) gcr_desired = 0;
                else gcr_desired -= LED_GCR_STEP;
                if (led_animation_breathing) gcr_breathe = gcr_desired;
            }
            return false;
        case L_PTN:
            if (record->event.pressed) {
                if (led_animation_id == led_setups_count - 1) led_animation_id = 0;
                else led_animation_id++;
            }
            return false;
        case L_PTP:
            if (record->event.pressed) {
                if (led_animation_id == 0) led_animation_id = led_setups_count - 1;
                else led_animation_id--;
            }
            return false;
        case L_PSI:
            if (record->event.pressed) {
                led_animation_speed += ANIMATION_SPEED_STEP;
            }
            return false;
        case L_PSD:
            if (record->event.pressed) {
                led_animation_speed -= ANIMATION_SPEED_STEP;
                if (led_animation_speed < 0) led_animation_speed = 0;
            }
            return false;
        case L_T_MD:
            if (record->event.pressed) {
                led_lighting_mode++;
                if (led_lighting_mode > LED_MODE_MAX_INDEX) led_lighting_mode = LED_MODE_NORMAL;
            }
            return false;
        case L_T_ONF:
            if (record->event.pressed) {
                led_enabled = !led_enabled;
                I2C3733_Control_Set(led_enabled);
            }
            return false;
        case L_ON:
            if (record->event.pressed) {
                led_enabled = 1;
                I2C3733_Control_Set(led_enabled);
            }
            return false;
        case L_OFF:
            if (record->event.pressed) {
                led_enabled = 0;
                I2C3733_Control_Set(led_enabled);
            }
            return false;
        case L_T_BR:
            if (record->event.pressed) {
                led_animation_breathing = !led_animation_breathing;
                if (led_animation_breathing) {
                    gcr_breathe = gcr_desired;
                    led_animation_breathe_cur = BREATHE_MIN_STEP;
                    breathe_dir = 1;
                }
            }
            return false;
        case L_T_PTD:
            if (record->event.pressed) {
                led_animation_direction = !led_animation_direction;
            }
            return false;
        case U_T_AUTO:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_extra_manual, "USB extra port manual mode");
            }
            return false;
        case U_T_AGCR:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_gcr_auto, "USB GCR auto mode");
            }
            return false;
        case DBG_TOG:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_enable, "Debug mode");
            }
            return false;
        case DBG_MTRX:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_matrix, "Debug matrix");
            }
            return false;
        case DBG_KBD:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_keyboard, "Debug keyboard");
            }
            return false;
        case DBG_MOU:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_mouse, "Debug mouse");
            }
            return false;
        case MD_BOOT:
            if (record->event.pressed) {
                key_timer = timer_read32();
            } else {
                if (timer_elapsed32(key_timer) >= 500) {
                    reset_keyboard();
                }
            }
            return false;
        
        
    
        }
}
            return true; //Process all other keycodes normally
}
