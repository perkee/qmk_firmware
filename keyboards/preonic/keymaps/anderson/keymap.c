/* Copyright 2015-2017 Jack Humbert
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "preonic.h"
#include "action_layer.h"
#include "musical_notes.h"

#include "scheduler.h"

// Helpers

#define IS_MOD_HIT(k) (bool)(keyboard_report->mods & MOD_BIT(k))
#define HEXCHAR(n) n ? (n < 10 ? (KC_1 + n - 1) : (KC_A + (n - 10))) : KC_0
const uint8_t UUID_MAC[] = {0xD, 0xE, 0xA, 0xD, 0xC, 0x0, 0xD, 0xE, 0x1, 0x3, 0x3, 0x7};

// Sounds

float LOWER_SND[][3][2] = {
    {S__NOTE(_C5), S__NOTE(_G4), S__NOTE(_C4)},
    {S__NOTE(_C5), S__NOTE(_REST), S__NOTE(_REST)}
};
float RAISE_SND[][3][2] = {
    {S__NOTE(_C5), S__NOTE(_G5), S__NOTE(_C6)},
    {S__NOTE(_C5), S__NOTE(_REST), S__NOTE(_REST)},
};
float SHIFT_SND[][1][2] = {
    {E__NOTE(_E3)},
    {E__NOTE(_E3)},
};
float ALT_SND[][1][2] = {
    {E__NOTE(_G3)},
    {E__NOTE(_G3)},
};
float CTRL_SND[][1][2] = {
    {E__NOTE(_B3)},
    {E__NOTE(_B3)},
};
float SHOTGUN_KEY_SND[][2][2] = {
    {S__NOTE(_D5), S__NOTE(_D6)},
    {S__NOTE(_D6), S__NOTE(_D5)}
};
float LANG_SWITCH_SOUND[][2] = {
    S__NOTE(_C6), S__NOTE(_C5), S__NOTE(_C6)
};
float LOCK_SND[][2] = {
    E__NOTE(_C5), E__NOTE(_G5), E__NOTE(_C6), E__NOTE(_G6), E__NOTE(_C7)
};
float CTRL_BREAK_SND[][2] = {
    S__NOTE(_A4), S__NOTE(_REST), S__NOTE(_GS4), S__NOTE(_REST), S__NOTE(_G4)
};

float BACKLIGHT_SOUNDS[][3][2] = {
    {S__NOTE(_C3)},
    {S__NOTE(_C4)},
    {S__NOTE(_C5)},
    {Q__NOTE(_C6)},
};

/*float KEY_SOUND[][2] = {*/
/*    S__NOTE(_C3)*/
/*};*/

// "Shotgun" keys.
// When tapped, send 1st key.
// When held & released, send 2nd key.

#define SHOTGUN_LEN 10
#define SHOTGUN_DELAY 200

uint16_t shotgun[SHOTGUN_LEN][3] = {
    {KC_1, KC_F1, 0},
    {KC_2, KC_F2, 0},
    {KC_3, KC_F3, 0},
    {KC_4, KC_F4, 0},
    {KC_5, KC_F5, 0},
    {KC_6, KC_F6, 0},
    {KC_7, KC_F7, 0},
    {KC_8, KC_F8, 0},
    {KC_9, KC_F9, 0},
    {KC_0, KC_F10, 0}
};

/*static void shotgun_timeout(int pair_index) {*/
/*    uint16_t code = shotgun[pair_index][1];*/
/*    register_code(code);*/
/*    unregister_code(code);*/
/*}*/

// Scheduler

void matrix_scan_user(void) {
    /*scheduler_process();*/
}

// Random byte
uint8_t randhex(void) {
    #if defined(__AVR_ATmega32U4__)
        uint8_t key = (TCNT0 + TCNT1 + TCNT3 + TCNT4) % 16;
    #else
        uint8_t key = rand() % 16;
    #endif
    return key;
}

// Actual keys

enum preonic_layers {
    _QWERTY,
    _LOWER,
    _RAISE,
    _ADJUST
};

enum preonic_keycodes {
    QWERTY = SAFE_RANGE,
    LOWER,
    RAISE,
    BACKLIT,
    UUIDGEN
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
    ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
    ┃ ESC ┃  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  ┃ BSP ┃
    ┣━━━━━╉-----+-----+-----+-----+-----+-----+-----+-----+-----+-----╊━━━━━┫
    ┃ TAB ┃  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  ┃  \  ┃
    ┣━━━━━╉-----+-----+-----+-----+-----+-----+-----+-----+-----+-----╊━━━━━┫
    ┃LOWER┃  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  ┃ RET ┃
    ┣━━━━━╉-----+-----+-----+-----+-----+-----+-----+-----+-----+-----╊━━━━━┫
    ┃SHIFT┃  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  .  |  ,  |  /  ┃ CTL ┃
    ┣━━━━━╉-----+-----+-----+-----┏━━━━━━━━━━━┓-----+-----+-----+-----╊━━━━━┫
    ┃RAISE┃ CTL | MOD | ALT |RAISE┃   SPACE   ┃LOWER|  "  |  '  |  -  ┃  =  ┃
    ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━━━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
*/
[_QWERTY] = {
    {KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC },
    {KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS },
    {LOWER,   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_ENT  },
    {KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_LCTL },
    {RAISE,   KC_LCTL, KC_LGUI, KC_LALT, RAISE,   KC_SPC,  KC_SPC,  LOWER,   KC_DQUO, KC_QUOT, KC_MINS, KC_EQL  }
},

/*
    ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
    ┃  ~  ┃     |     |     |     |     |     |     |     |  -  |  =  ┃ DEL ┃
    ┣━━━━━╉-----+-----+-----+-----+-----+-----+-----+-----+-----+-----╊━━━━━┫
    ┃ TAB ┃PREV |PLAY |NEXT |     |     | INS |PG UP|  ▲  |PG DN|  ~  ┃  `  ┃
    ┣━━━━━╉-----+-----+-----+-----+-----+-----+-----+-----+-----+-----╊━━━━━┫
    ┃LOWER┃     |VOL +|VOL -|     |     |HOME |  ◀  |  ▼  |  ▶  |     ┃     ┃
    ┣━━━━━╉-----+-----+-----+-----+-----+-----+-----+-----+-----+-----╊━━━━━┫
    ┃     ┃     |     |     |     |     | END |     |  [  |  ]  |     ┃     ┃
    ┣━━━━━╉-----+-----+-----+-----┏━━━━━━━━━━━┓-----+-----+-----+-----╊━━━━━┫
    ┃     ┃     |     |     |     ┃           ┃     |  {  |  }  |     ┃UUID ┃
    ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━━━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
*/
[_LOWER] = {
    {KC_TILD, _______, _______, _______, _______, _______, _______, _______, _______, KC_MINS, KC_EQL,  KC_DEL },
    {KC_TAB,  KC_MPRV, KC_MPLY, KC_MNXT, _______, _______, KC_INS,  KC_PGUP, KC_UP,   KC_PGDN, KC_TILD, KC_GRV },
    {LOWER,   _______, KC_VOLD, KC_VOLU, _______, _______, KC_HOME, KC_LEFT, KC_DOWN, KC_RIGHT,_______, _______},
    {_______, _______, _______, _______, _______, _______, KC_END,  _______, KC_LBRC, KC_RBRC, _______, _______},
    {_______, _______, _______, _______, _______, _______, _______, _______, KC_LCBR, KC_RCBR, _______, UUIDGEN}
},

/*
    ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
    ┃     ┃ F1  | F2  | F3  | F4  | F5  | F6  | F7  | F8  | F9  | F10 ┃     ┃
    ┣━━━━━╉-----+-----+-----+-----+-----+-----+-----+-----+-----+-----╊━━━━━┫
    ┃LIGHT┃     |     |     |     |     |     |     |     | F11 | F12 ┃     ┃
    ┣━━━━━╉-----+-----+-----+-----+-----+-----+-----+-----+-----+-----╊━━━━━┫
    ┃LOWER┃     |     |           |     |     |     |     |     |     ┃     ┃
    ┣━━━━━╉-----+-----+-----+-----+-----+-----+-----+-----+-----+-----╊━━━━━┫
    ┃     ┃     |     |     |     |     |     |     |HOME |PG UP|PG DN┃ END ┃
    ┣━━━━━╉-----+-----+-----+-----┏━━━━━━━━━━━┓-----+-----+-----+-----╊━━━━━┫
    ┃     ┃     |     |     |     ┃           ┃     |  ◀  |  ▼  |  ▲  ┃  ▶  ┃
    ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━━━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
*/
[_RAISE] = {
    {_______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______},
    {BACKLIT, _______, _______, _______, _______, _______, _______, _______, _______, KC_F11,  KC_F12,  _______},
    {LOWER,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______},
    {_______, _______, _______, _______, _______, _______, _______, _______, KC_HOME, KC_PGUP, KC_PGDN, KC_END},
    {_______, _______, _______, _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP,  KC_RIGHT}
},

/*
    ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
    ┃     ┃     |     |     |     |     |     |     |     |     |     ┃     ┃
    ┣━━━━━╉-----+-----+-----+-----+-----+-----+-----+-----+-----+-----╊━━━━━┫
    ┃LIGHT┃RESET|DEBUG|     |     |     |     |TERM+|TERM-|     |     ┃     ┃
    ┣━━━━━╉-----+-----+-----+-----+-----+-----+-----+-----+-----+-----╊━━━━━┫
    ┃     ┃     |     |SND+ |SND- |     |     |     |     |     |     ┃     ┃
    ┣━━━━━╉-----+-----+-----+-----+-----+-----+-----+-----+-----+-----╊━━━━━┫
    ┃     ┃     |     |MUS+ |MUS- |MIDI+|MIDI-|     |     |     |     ┃     ┃
    ┣━━━━━╉-----+-----+-----+-----┏━━━━━━━━━━━┓-----+-----+-----+-----╊━━━━━┫
    ┃     ┃     |     |     |     ┃           ┃     |     |     |     ┃     ┃
    ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━━━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
*/
[_ADJUST] = {
    {_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______},
    {BACKLIT, RESET,   DEBUG,   _______, _______, _______, _______, TERM_ON, TERM_OFF,_______, _______, _______},
    {_______, _______, _______, AU_ON,   AU_OFF,  _______, _______, _______, _______, _______, _______, _______},
    {_______, _______, _______, MU_ON,   MU_OFF,  MI_ON,   MI_OFF,  _______, _______, _______, _______, _______},
    {_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______}
},
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QWERTY:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QWERTY);
            }
            return false;
        case LOWER:
            if (record->event.pressed) {
                PLAY_SONG(LOWER_SND[0]);
                layer_on(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
        case RAISE:
            if (record->event.pressed) {
                PLAY_SONG(RAISE_SND[0]);
                layer_on(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
        case BACKLIT:
            if (record->event.pressed) {
                register_code(KC_RSFT);
                #ifdef BACKLIGHT_ENABLE
                    backlight_step();
                    PLAY_SONG(BACKLIGHT_SOUNDS[get_backlight_level()]);
                #endif
                PORTE &= ~(1<<6);
            } else {
                unregister_code(KC_RSFT);
                PORTE |= (1<<6);
            }
            return false;
        case KC_LALT:
            (record->event.pressed ? register_code : unregister_code)(keycode);
            if (record->event.pressed) {
                PLAY_SONG(ALT_SND[1]);
                if (IS_MOD_HIT(KC_LSHIFT)) PLAY_SONG(LANG_SWITCH_SOUND);
            }
            return false;
        case KC_LSHIFT:
            (record->event.pressed ? register_code : unregister_code)(keycode);
            if (record->event.pressed) {
                PLAY_SONG(SHIFT_SND[1]);
                if (IS_MOD_HIT(KC_LALT)) PLAY_SONG(LANG_SWITCH_SOUND);
            }
            return false;
        case KC_LCTRL:
        case KC_RCTRL:
            (record->event.pressed ? register_code : unregister_code)(keycode);
            if (record->event.pressed) {
                PLAY_SONG(CTRL_SND[1]);
            }
            return false;
        case KC_L:
            (record->event.pressed ? register_code : unregister_code)(keycode);
            if (record->event.pressed && IS_MOD_HIT(KC_LALT) && (IS_MOD_HIT(KC_LCTRL) || IS_MOD_HIT(KC_RCTRL))) {
                PLAY_SONG(LOCK_SND);
            }
            return false;
        case KC_C:
            (record->event.pressed ? register_code : unregister_code)(keycode);
            if (record->event.pressed && (IS_MOD_HIT(KC_LCTRL) || IS_MOD_HIT(KC_RCTRL)) && !IS_MOD_HIT(KC_LSHIFT)) {
                PLAY_SONG(CTRL_BREAK_SND);
            }
            return false;
        case UUIDGEN:
            if (record->event.pressed) {
                bool upper = false;
                if (IS_MOD_HIT(KC_LSHIFT)) {
                    unregister_code(KC_LSHIFT);
                    upper = true;
                }
                for (int i = 0; i < 32; i++) {
                    uint8_t hex;
                    if (i > 19) {
                        hex = UUID_MAC[i - 20];
                    } else {
                        hex = randhex();
                    }
                    if (hex >= 10 && upper) {
                        register_code(KC_LSHIFT);
                    }
                    register_code(HEXCHAR(hex));
                    unregister_code(HEXCHAR(hex));
                    if (hex >= 10 && upper) {
                        unregister_code(KC_LSHIFT);
                    }
                    if (i == 7 || i == 11 || i == 15 || i == 19) {
                        register_code(KC_MINS);
                        unregister_code(KC_MINS);
                    }
                }
            }
            return false;
        /*default:*/
        /*    PLAY_SONG(KEY_SOUND);*/
    }
    for (int i = 0; i < SHOTGUN_LEN; i++) {
        uint16_t *pair = shotgun[i];
        uint16_t code;
        if (pair[0] == keycode) {
            if (record->event.pressed) {
                /*pair[2] = run_after(SHOTGUN_DELAY, shotgun_timeout, i);*/
                pair[2] = timer_read();
            } else {
                /*if (!cancelled(pair[2])) {*/
                /*    // Premature raise, i. e. tap*/
                /*    cancel(pair[2]);*/
                /*    register_code(keycode);*/
                /*    unregister_code(keycode);*/
                /*}*/
                if (timer_elapsed(pair[2]) > SHOTGUN_DELAY) {
                    code = pair[1];
                    PLAY_SONG(SHOTGUN_KEY_SND[0]);
                } else {
                    code = pair[0];
                }
                register_code(code);
                unregister_code(code);
            }
            return false;
        }
    }
    return true;
};

