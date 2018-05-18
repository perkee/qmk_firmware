// Six-Seven - minimal layout for Let's Split.
#include "lets_split.h"
#include "action_layer.h"
#include "eeconfig.h"

extern keymap_config_t keymap_config;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _MAIN 0
#define _ALPHA 1
#define _BETA 2

enum custom_keycodes {
  MAIN = SAFE_RANGE,
  ALPHA,
  BETA
};

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO
#define SH(x) LSFT(KC_##x)

// α/ESC timeout.
// If α is tapped for less than this value, send ESC in addition to enabling α layer.
#define ALPHA_ESC_TIMEOUT 90

// TAP shortcut
#define TAP(key) register_code(key); unregister_code(key)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Main layer
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┓      ┏━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃ TAB ┃  Q  |  W  |  E  |  R  |  T  ┃      ┃  Y  |  U  |  I  |  O  |  P  ┃ BSP ┃
       ┣━━━━━╉-----+-----+-----+-----+-----┫      ┣-----+-----+-----+-----+-----╊━━━━━┫
       ┃α/ESC┃  A  |  S  |  D  |  F  |  G  ┃      ┃  H  |  J  |  K  |  L  |  ;  ┃ RET ┃
       ┣━━━━━╉-----+-----+-----+-----+-----┫      ┣-----+-----+-----+-----+-----╊━━━━━┫
       ┃SHIFT┃  Z  |  X  |  C  |  V  |  B  ┃      ┃  N  |  M  |  ,  |  .  |  /  ┃ CTL ┃
       ┗━━━━━┻━━━━━━━━━━━┳━━━━━┳━━━━━┳━━━━━┫      ┣━━━━━┳━━━━━┳━━━━━┳━━━━━━━━━━━┻━━━━━┛
                         ┃ MOD ┃ ALT ┃SPACE┃      ┃SPACE┃  β  ┃  '  ┃
                         ┗━━━━━┻━━━━━┻━━━━━┛      ┗━━━━━┻━━━━━┻━━━━━┛
       */
    [_MAIN] = LAYOUT( \
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC, \
        ALPHA,   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,             KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_ENT,  \
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,             KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_LCTRL,\
        _______, _______, _______, KC_LGUI, KC_LALT, KC_SPC,           KC_SPC,  BETA,    KC_QUOT, _______, _______, _______  \
    ),

    /* Alpha layer (α)
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┓      ┏━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃     ┃PREV |PLAY |NEXT |     |     ┃      ┃     | ^^^ |  ^  | vvv |  ~  ┃ DEL ┃
       ┣━━━━━╉-----+-----+-----+-----+-----┫      ┣-----+-----+-----+-----+-----╊━━━━━┫
       ┃     ┃     |VOL -|VOL +|     |     ┃      ┃HOME | <-- |  v  | --> |  `  ┃  \  ┃
       ┣━━━━━╉-----+-----+-----+-----+-----┫      ┣-----+-----+-----+-----+-----╊━━━━━┫
       ┃     ┃     |     |     |     |     ┃      ┃ END |     |  [  |  ]  |  (  ┃  )  ┃
       ┗━━━━━┻━━━━━━━━━━━┳━━━━━┳━━━━━┳━━━━━┫      ┣━━━━━┳━━━━━┳━━━━━┳━━━━━━━━━━━┻━━━━━┛
                         ┃     ┃     ┃     ┃      ┃     ┃     ┃     ┃
                         ┗━━━━━┻━━━━━┻━━━━━┛      ┗━━━━━┻━━━━━┻━━━━━┛
       */
    [_ALPHA] = LAYOUT( \
        _______, KC_MPRV, KC_MPLY, KC_MNXT, _______, _______,          _______, KC_PGUP, KC_UP,   KC_PGDN, KC_TILD, KC_DEL,  \
        _______, _______, KC_VOLD, KC_VOLU, _______, _______,          KC_HOME, KC_LEFT, KC_DOWN, KC_RIGHT,KC_GRV,  KC_BSLS, \
        _______, _______, _______, _______, _______, _______,          KC_END,  _______, KC_LBRC, KC_RBRC, SH(9),   SH(0),   \
        _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______  \
    ),

    /* Beta layer (β)
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┓      ┏━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃  `  ┃  1  |  2  |  3  |  4  |  5  ┃      ┃  6  |  7  |  8  |  9  |  0  ┃ del ┃
       ┣━━━━━╉-----+-----+-----+-----+-----┫      ┣-----+-----+-----+-----+-----╊━━━━━┫
       ┃     ┃ F1  | F2  | F3  | F4  | F5  ┃      ┃ F6  | F7  | F8  | F9  | F10 ┃  |  ┃
       ┣━━━━━╉-----+-----+-----+-----+-----┫      ┣-----+-----+-----+-----+-----╊━━━━━┫
       ┃     ┃RESET|DEBUG|     | F11 | F12 ┃      ┃SND +|SND -|  {  |  }  |  -  ┃  =  ┃
       ┗━━━━━┻━━━━━━━━━━━┳━━━━━┳━━━━━┳━━━━━┫      ┣━━━━━┳━━━━━┳━━━━━┳━━━━━━━━━━━┻━━━━━┛
                         ┃     ┃     ┃     ┃      ┃     ┃     ┃     ┃
                         ┗━━━━━┻━━━━━┻━━━━━┛      ┗━━━━━┻━━━━━┻━━━━━┛
       */
    [_BETA] = LAYOUT( \
        _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,             KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL,  \
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,            KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_PIPE, \
        _______, RESET,   DEBUG,   _______, KC_F11,  KC_F12,           AU_ON,   AU_OFF,  KC_LCBR, KC_RCBR, KC_MINS, KC_EQUAL,\
        _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______  \
    )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint16_t alpha_esc_start = 0;
    switch (keycode) {
        case ALPHA:
            if (record->event.pressed) {
                layer_on(_ALPHA);
                alpha_esc_start = timer_read();
            } else {
                layer_off(_ALPHA);
                if (timer_elapsed(alpha_esc_start) < ALPHA_ESC_TIMEOUT) {
                    TAP(KC_ESC);
                }
            }
            return false;
        case BETA:
            if (record->event.pressed) {
                layer_on(_BETA);
            } else {
                layer_off(_BETA);
            }
            return false;
    }
    return true;
}
