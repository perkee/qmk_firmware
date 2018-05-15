#include "config.h"
#include "musical_notes.h"
#include "lets_split.h"
#include "action_layer.h"
#include "eeconfig.h"
#include "pincontrol.h"
#include "pointing_device.h"
#include "avr/interrupt.h"

/*#include "solid.h"*/
#include "dmc12.h"
#include "rainbow.h"
#include "custom_sounds.h"

/*#include "knob.h"*/
#include "knob_v2.h"

extern keymap_config_t keymap_config;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _QWERTY 0
#define _ALPHA 1
#define _BETA 2
#define _GAMMA 3

#define IS_MOD_HIT(k) (bool)(keyboard_report->mods & MOD_BIT(k))
/*bool IS_MOD_HIT(uint16_t k) {*/
    /*return keyboard_report->mods & MOD_BIT(k);*/
/*}*/

enum custom_keycodes {
    QWERTY = SAFE_RANGE,
    ALPHA,
    BETA,
    GAMMA
};

// Highlights

enum HL {
    /*SOLID,*/
    DMC12,
    RAINBOW
};

enum HL current_hl = RAINBOW;

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO

// Alpha/ESC timeout.
// If ALPHA is tapped for less than this value, send ESC in addition to ALPHA.
#define ALPHA_ESC_TIMEOUT 85
uint16_t alpha_esc_start;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* Qwerty
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┓      ┏━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃ TAB ┃  Q  |  W  |  E  |  R  |  T  ┃      ┃  Y  |  U  |  I  |  O  |  P  ┃ BSP ┃
       ┣━━━━━╉-----+-----+-----+-----+-----┫      ┣-----+-----+-----+-----+-----╊━━━━━┫
       ┃L/ESC┃  A  |  S  |  D  |  F  |  G  ┃      ┃  H  |  J  |  K  |  L  |  ;  ┃ RET ┃
       ┣━━━━━╉-----+-----+-----+-----+-----┫      ┣-----+-----+-----+-----+-----╊━━━━━┫
       ┃SHIFT┃  Z  |  X  |  C  |  V  |  B  ┃      ┃  N  |  M  |  .  |  ,  |  /  ┃ CTL ┃
       ┣━━━━━╉-----+-----+-----+-----┏━━━━━┫      ┣━━━━━┓-----+-----+-----+-----╊━━━━━┫
       ┃GAMMA┃ CTL | MOD | ALT |ALPHA┃SPACE┃      ┃SPACE┃BETA |  "  |  '  |  -  ┃  =  ┃
       ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛      ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
       */
    [_QWERTY] = LAYOUT( \
            KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC, \
            ALPHA,   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,             KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_ENT,  \
            KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,             KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_LCTRL,\
            GAMMA,   KC_LCTRL,KC_LGUI, KC_LALT, ALPHA,   KC_SPC,           KC_SPC,  BETA,    KC_DQUO, KC_QUOTE,KC_MINUS,KC_EQUAL \
            ),

    /* Alpha
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
    [_ALPHA] = LAYOUT( \
            KC_TILD, KC_MPRV, KC_MPLY, KC_MNXT, _______, _______,          _______, KC_PGUP, KC_UP,   KC_PGDN, KC_TILD, KC_DEL,  \
            _______, _______, _______, _______, _______, _______,          KC_HOME, KC_LEFT, KC_DOWN, KC_RIGHT,KC_GRV,  KC_BSLS, \
            _______, _______, _______, _______, _______, _______,          KC_END,  _______, KC_LBRC, KC_RBRC, LSFT(KC_9),LSFT(KC_0),\
            _______, _______, _______, _______, _______, _______,          _______, _______, KC_LCBR, KC_RCBR, KC_MINS, KC_EQUAL \
            ),

    /* Beta
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┓      ┏━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃  `  ┃  1  |  2  |  3  |  4  |  5  ┃      ┃  6  |  7  |  8  |  9  |  0  ┃ del ┃
       ┣━━━━━╉-----+-----+-----+-----+-----┫      ┣-----+-----+-----+-----+-----╊━━━━━┫
       ┃     ┃ F1  | F2  | F3  | F4  | F5  ┃      ┃ F6  | F7  | F8  | F9  | F10 ┃     ┃
       ┣━━━━━╉-----+-----+-----+-----+-----┫      ┣-----+-----+-----+-----+-----╊━━━━━┫
       ┃     ┃RESET|DEBUG|     |     |     ┃      ┃SND +|SND -|     | F11 | F12 ┃     ┃
       ┣━━━━━╉-----+-----+-----+-----┏━━━━━┫      ┣━━━━━┓-----+-----+-----+-----╊━━━━━┫
       ┃     ┃     |     |     |     ┃     ┃      ┃     ┃     |     |     |     ┃     ┃
       ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛      ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
       */
    [_BETA] = LAYOUT( \
            KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,             KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL,  \
            _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,            KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______, \
            _______, RESET,   DEBUG,   _______, _______, _______,          AU_ON,   AU_OFF,  _______, KC_F11,  KC_F12,  _______, \
            _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______  \
            ),
    /* Gamma
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┓      ┏━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃     ┃     |     |     |     |     ┃      ┃     |     |     |     |     ┃     ┃
       ┣━━━━━╉-----+-----+-----+-----+-----┫      ┣-----+-----+-----+-----+-----╊━━━━━┫
       ┃     ┃     |     |     |     |     ┃      ┃     |     |     |     |     ┃     ┃
       ┣━━━━━╉-----+-----+-----+-----+-----┫      ┣-----+-----+-----+-----+-----╊━━━━━┫
       ┃     ┃     |     |     |     |     ┃      ┃     |     |     |     |     ┃     ┃
       ┣━━━━━╉-----+-----+-----+-----┏━━━━━┫      ┣━━━━━┓-----+-----+-----+-----╊━━━━━┫
       ┃     ┃     |     |     |     ┃     ┃      ┃     ┃     |     |     |     ┃     ┃
       ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛      ┗━━━━━┻━━━━━┷━━━━━┷━━━━━┷━━━━━┻━━━━━┛
       */
    [_GAMMA] = LAYOUT( \
            _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, \
            _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, \
            _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, \
            _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______  \
            ),
};

void persistent_default_layer_set(uint16_t default_layer) {
    eeconfig_update_default_layer(default_layer);
    default_layer_set(default_layer);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case ALPHA:
            if (record->event.pressed) {
#ifdef KEY_SOUNDS
                PLAY_SONG(ALPHA_SND);
#endif
                alpha_esc_start = timer_read();
                layer_on(_ALPHA);
                /*update_tri_layer(_ALPHA, _BETA, _ADJUST);*/
            } else {
                layer_off(_ALPHA);
                /*update_tri_layer(_ALPHA, _BETA, _ADJUST);*/
                if (timer_elapsed(alpha_esc_start) < ALPHA_ESC_TIMEOUT) {
#ifdef KEY_SOUNDS
                    PLAY_SONG(ESC_SND);
#endif
                    register_code(KC_ESC);
                    unregister_code(KC_ESC);
                }
            }
            break;
        case BETA:
            if (record->event.pressed) {
#ifdef KEY_SOUNDS
                PLAY_SONG(BETA_SND);
#endif
                layer_on(_BETA);
                /*update_tri_layer(_ALPHA, _BETA, _ADJUST);*/
            } else {
                layer_off(_BETA);
                /*update_tri_layer(_ALPHA, _BETA, _ADJUST);*/
            }
            break;
        /*case ADJUST:*/
        /*    if (record->event.pressed) {*/
        /*        layer_on(_ADJUST);*/
        /*    } else {*/
        /*        layer_off(_ADJUST);*/
        /*    }*/
        /*    return false;*/
        /*    break;*/
        case GAMMA:
            if (record->event.pressed) {
#ifdef KEY_SOUNDS
                /*PLAY_SONG(GAMMA_SND);*/
#endif
                layer_on(_GAMMA);
                /*update_tri_layer(_ALPHA, _BETA, _ADJUST);*/
            } else {
                layer_off(_GAMMA);
                /*update_tri_layer(_ALPHA, _BETA, _ADJUST);*/
            }
            break;
#ifdef KEY_SOUNDS
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
#endif
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
        /*case SOLID:*/
            /*solid_process();*/
            /*break;*/
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
            if (layer_state_is(_BETA)) {
                register_code(KC_VOLD);
                unregister_code(KC_VOLD);
            } else if (layer_state_is(_ALPHA)) {
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
            if (layer_state_is(_BETA)) {
                register_code(KC_VOLU);
                unregister_code(KC_VOLU);
            } else if (layer_state_is(_ALPHA)) {
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
        case _ALPHA:
            current_hl = DMC12;
            dmc12_start(0xFF0000);
            break;
        case _BETA:
            current_hl = DMC12;
            dmc12_start(0x0000FF);
            break;
        case _GAMMA:
            current_hl = DMC12;
            dmc12_start(0x7F00FF);
            break;
        /*case _ADJUST:*/
        /*    current_hl = DMC12;*/
        /*    dmc12_start(0x7F00FF);*/
        /*    break;*/
    }
    return state;
}
