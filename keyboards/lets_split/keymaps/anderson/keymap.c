#include "config.h"
#include "lets_split.h"
#include "action_layer.h"
#include "eeconfig.h"
#include "pincontrol.h"
#include "pointing_device.h"
#include "avr/interrupt.h"

#include "solid.h"
#include "dmc12.h"
#include "rainbow.h"

/*#include "knob.h"*/
#include "knob_v2.h"

extern keymap_config_t keymap_config;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _QWERTY 0
#define _LOWER 1
#define _RAISE 2
#define _ADJUST 3

#define IS_MOD_HIT(k) (bool)(keyboard_report->mods & MOD_BIT(k))

float LOWER_SND[][2] = {
    S__NOTE(_C5), S__NOTE(_G4), S__NOTE(_C4)
};
float RAISE_SND[][2] = {
    S__NOTE(_C5), S__NOTE(_G5), S__NOTE(_C6)
};
float SHIFT_SND[][2] = {
    E__NOTE(_E3)
};
float ALT_SND[][2] = {
    E__NOTE(_G3)
};
float CTRL_SND[][2] = {
    E__NOTE(_B3)
};
float ESC_SND[][2] = {
    S__NOTE(_D5), S__NOTE(_D6)
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

enum custom_keycodes {
    QWERTY = SAFE_RANGE,
    LOWER,
    RAISE,
    ADJUST,
    LOW_ESC
};

// Highlights

enum HL {
    SOLID,
    DMC12,
    RAINBOW
};

enum HL current_hl = RAINBOW;

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO

// Lower/ESC timeout.
// If LOW_ESC is tapped for less than this value, send ESC in addition to LOWER.
#define LOW_ESC_TIMEOUT 85
uint16_t low_esc_start;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* Qwerty
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┓      ┏━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃ TAB ┃  Q  |  W  |  E  |  R  |  T  ┃      ┃  Y  |  U  |  I  |  O  |  P  ┃ BSP ┃
       ┣━━━━━╉-----+-----+-----+-----+-----┫      ┣-----+-----+-----+-----+-----╊━━━━━┫
       ┃L/ESC┃  A  |  S  |  D  |  F  |  G  ┃      ┃  H  |  J  |  K  |  L  |  ;  ┃ RET ┃
       ┣━━━━━╉-----+-----+-----+-----+-----┫      ┣-----+-----+-----+-----+-----╊━━━━━┫
       ┃SHIFT┃  Z  |  X  |  C  |  V  |  B  ┃      ┃  N  |  M  |  .  |  ,  |  /  ┃ CTL ┃
       ┣━━━━━╉-----+-----+-----+-----┏━━━━━┫      ┣━━━━━┓-----+-----+-----+-----╊━━━━━┫
       ┃RAISE┃ CTL | MOD | ALT |LOWER┃SPACE┃      ┃SPACE┃RAISE|  "  |  '  |  -  ┃  =  ┃
       ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛      ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
       */
    [_QWERTY] = LAYOUT( \
            KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC, \
            LOW_ESC, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,             KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_ENT,  \
            KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,             KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_LCTRL,\
            RAISE,   KC_LCTRL,KC_LGUI, KC_LALT, LOWER,   KC_SPC,           KC_SPC,  RAISE,   KC_DQUO, KC_QUOTE,KC_MINUS,KC_EQUAL \
            ),

    /* Lower
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┓      ┏━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃  ~  ┃PREV |PLAY |NEXT |     |     ┃      ┃     | ^^^ |  ^  | vvv |  ~  ┃ DEL ┃
       ┣━━━━━╉-----+-----+-----+-----+-----┫      ┣-----+-----+-----+-----+-----╊━━━━━┫
       ┃     ┃     |     |     |     |     ┃      ┃HOME | <-- |  v  | --> |  `  ┃  \  ┃
       ┣━━━━━╉-----+-----+-----+-----+-----┫      ┣-----+-----+-----+-----+-----╊━━━━━┫
       ┃     ┃     |     |     |     |     ┃      ┃ END |     |  [  |  ]  |  (  ┃  )  ┃
       ┣━━━━━╉-----+-----+-----+-----┏━━━━━┫      ┣━━━━━┓-----+-----+-----+-----╊━━━━━┫
       ┃     ┃     |     |     |     ┃     ┃      ┃     ┃     |  {  |  }  |  -  ┃  =  ┃
       ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛      ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
       */
    [_LOWER] = LAYOUT( \
            KC_TILD, KC_MPRV, KC_MPLY, KC_MNXT, _______, _______,          _______, KC_PGUP, KC_UP,   KC_PGDN, KC_TILD, KC_DEL,  \
            _______, _______, _______, _______, _______, _______,          KC_HOME, KC_LEFT, KC_DOWN, KC_RIGHT,KC_GRV,  KC_BSLS, \
            _______, _______, _______, _______, _______, _______,          KC_END,  _______, KC_LBRC, KC_RBRC, LSFT(KC_9),LSFT(KC_0),\
            _______, _______, _______, _______, _______, _______,          _______, _______, KC_LCBR, KC_RCBR, KC_MINS, KC_EQUAL \
            ),

    /* Raise
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┓      ┏━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃  `  ┃  1  |  2  |  3  |  4  |  5  ┃      ┃  6  |  7  |  8  |  9  |  0  ┃ del ┃
       ┣━━━━━╉-----+-----+-----+-----+-----┫      ┣-----+-----+-----+-----+-----╊━━━━━┫
       ┃     ┃ F1  | F2  | F3  | F4  | F5  ┃      ┃ F6  | F7  | F8  | F9  | F10 ┃     ┃
       ┣━━━━━╉-----+-----+-----+-----+-----┫      ┣-----+-----+-----+-----+-----╊━━━━━┫
       ┃     ┃     |     |     |     |     ┃      ┃     |     |     | F11 | F12 ┃     ┃
       ┣━━━━━╉-----+-----+-----+-----┏━━━━━┫      ┣━━━━━┓-----+-----+-----+-----╊━━━━━┫
       ┃     ┃     |     |     |     ┃     ┃      ┃     ┃     |     |     |     ┃     ┃
       ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛      ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
       */
    [_RAISE] = LAYOUT( \
            KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,             KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL,  \
            _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,            KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______, \
            _______, _______, _______, _______, _______, _______,          _______, _______, _______, KC_F11,  KC_F12,  _______, \
            _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______  \
            ),

    /* Adjust (Lower + Raise)
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┓      ┏━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃     ┃RESET|     |     |     |     ┃      ┃     |     |     |     |     ┃     ┃
       ┣━━━━━╉-----+-----+-----+-----+-----┫      ┣-----+-----+-----+-----+-----╊━━━━━┫
       ┃     ┃     |     |SND -|SND +|     ┃      ┃     |     |     |     |     ┃     ┃
       ┣━━━━━╉-----+-----+-----+-----+-----┫      ┣-----+-----+-----+-----+-----╊━━━━━┫
       ┃     ┃     |     |     |     |     ┃      ┃     |     |     |     |     ┃     ┃
       ┣━━━━━╉-----+-----+-----+-----┏━━━━━┫      ┣━━━━━┓-----+-----+-----+-----╊━━━━━┫
       ┃     ┃     |     |     |     ┃     ┃      ┃     ┃     |     |     |     ┃     ┃
       ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛      ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
       */
    [_ADJUST] =  LAYOUT( \
            _______, RESET,   _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, \
            _______, _______, _______, AU_ON,   AU_OFF,  _______,          _______, _______, _______, _______, _______, _______, \
            _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, \
            _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______  \
            )


};

void persistent_default_layer_set(uint16_t default_layer) {
    eeconfig_update_default_layer(default_layer);
    default_layer_set(default_layer);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LOWER:
            if (record->event.pressed) {
                PLAY_SONG(LOWER_SND);
                layer_on(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
            break;
        case RAISE:
            if (record->event.pressed) {
                PLAY_SONG(RAISE_SND);
                layer_on(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
            break;
        case ADJUST:
            if (record->event.pressed) {
                layer_on(_ADJUST);
            } else {
                layer_off(_ADJUST);
            }
            return false;
            break;
        case LOW_ESC:
            if (record->event.pressed) {
                PLAY_SONG(LOWER_SND);
                low_esc_start = timer_read();
                layer_on(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
                if (timer_elapsed(low_esc_start) < LOW_ESC_TIMEOUT) {
                    PLAY_SONG(ESC_SND);
                    register_code(KC_ESC);
                    unregister_code(KC_ESC);
                }
            }
            break;
        case KC_LALT:
            if (record->event.pressed) {
                PLAY_SONG(ALT_SND);
                if (IS_MOD_HIT(KC_LSHIFT)) PLAY_SONG(LANG_SWITCH_SOUND);
            }
            break;
        case KC_LSHIFT:
            if (record->event.pressed) {
                PLAY_SONG(SHIFT_SND);
                if (IS_MOD_HIT(KC_LALT)) PLAY_SONG(LANG_SWITCH_SOUND);
            }
            break;
        case KC_LCTRL:
        case KC_RCTRL:
            if (record->event.pressed) {
                PLAY_SONG(CTRL_SND);
            }
            break;
        case KC_L:
            if (record->event.pressed && IS_MOD_HIT(KC_LALT) && (IS_MOD_HIT(KC_LCTRL) || IS_MOD_HIT(KC_RCTRL))) {
                PLAY_SONG(LOCK_SND);
            }
            break;
        case KC_C:
            if (record->event.pressed && (IS_MOD_HIT(KC_LCTRL) || IS_MOD_HIT(KC_RCTRL)) && !IS_MOD_HIT(KC_LSHIFT)) {
                PLAY_SONG(CTRL_BREAK_SND);
            }
            break;
    }
    return true;
}

void matrix_init_user(void) {
    /*solid_start(0xFF1177);*/
    rainbow_start();
#ifdef KNOB_ENABLE
    knob_init();
#endif
}

void matrix_scan_user(void) {
    switch (current_hl) {
        case SOLID:
            solid_process();
            break;
        case DMC12:
            dmc12_process();
            break;
        case RAINBOW:
            rainbow_process();
            break;
    }

#ifdef KNOB_ENABLE
    knob_report_t knob_report = knob_report_read();
    knob_report_reset();
    if (knob_report.phase) { // I check for phase to avoid handling the rotation twice (on 90 and 270 degrees).
        while (knob_report.dir > 0) {
            // CCW
            if (layer_state_is(_RAISE)) {
                register_code(KC_VOLD);
                unregister_code(KC_VOLD);
            } else if (layer_state_is(_LOWER)) {
                register_code(KC_LEFT);
                unregister_code(KC_LEFT);
            } else {
                report_mouse_t report = pointing_device_get_report();
                report.v += 1;
                pointing_device_set_report(report);
                /*register_code(KC_UP);*/
                /*unregister_code(KC_UP);*/
            }
            knob_report.dir--;
        }
        while (knob_report.dir < 0) {
            // CW
            if (layer_state_is(_RAISE)) {
                register_code(KC_VOLU);
                unregister_code(KC_VOLU);
            } else if (layer_state_is(_LOWER)) {
                register_code(KC_RIGHT);
                unregister_code(KC_RIGHT);
            } else {
                report_mouse_t report = pointing_device_get_report();
                report.v -= 1;
                pointing_device_set_report(report);
                /*register_code(KC_DOWN);*/
                /*unregister_code(KC_DOWN);*/
            }
            knob_report.dir++;
        }
    }
#endif
}

uint32_t layer_state_set_user(uint32_t state) {
    uint8_t layer = biton32(state);
    switch (layer) {
        case _QWERTY:
            current_hl = RAINBOW;
            rainbow_start();
            /*solid_start(0xFF1177);*/
            break;
        case _LOWER:
            current_hl = DMC12;
            dmc12_start(0xFF0000);
            break;
        case _RAISE:
            current_hl = DMC12;
            dmc12_start(0x0000FF);
            break;
        case _ADJUST:
            current_hl = DMC12;
            dmc12_start(0x7F00FF);
            break;
    }
    return state;
}
