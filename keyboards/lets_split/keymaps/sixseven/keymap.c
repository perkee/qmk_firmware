// Six-Seven - minimal layout for Let's Split.
#include "lets_split.h"
#include "action_layer.h"
#include "eeconfig.h"
#include "keys.h"
#include "pro_micro.h"

extern keymap_config_t keymap_config;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _MAIN 0
#define _ALPHA 1
#define _BETA 2

enum custom_keycodes {
  KC_MAIN = SAFE_RANGE,
  KC_ALPHA,
  KC_BETA,
  KC_D_CTRL_EQUAL
};

// Dual keys tap/hold timeout.
// If key is tapped for less than this value, send key in addition to primary action after completing the action.
#define DUAL_HOLD_TIMEOUT 90

// TAP shortcut
#define TAP(key) register_code(key); unregister_code(key)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Main layer
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┓      ┏━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃ TAB ┃  Q  │  W  │  E  │  R  │  T  ┃      ┃  Y  │  U  │  I  │  O  │  P  ┃ BSP ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨      ┠─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃α/ESC┃  A  │  S  │  D  │  F  │  G  ┃      ┃  H  │  J  │  K  │  L  │  ;  ┃ RET ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨      ┠─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃SHIFT┃  Z  │  X  │  C  │  V  │  B  ┃      ┃  N  │  M  │  ,  │  .  │  /  ┃ CTL ┃
       ┗━━━━━┻━━━━━┷━━━━━╈━━━━━╈━━━━━╈━━━━━┫      ┣━━━━━╈━━━━━╈━━━━━╈━━━━━┷━━━━━┻━━━━━┛
                         ┃ ALT ┃ MOD ┃SPACE┃      ┃SPACE┃  β  ┃  '  ┃
                         ┗━━━━━┻━━━━━┻━━━━━┛      ┗━━━━━┻━━━━━┻━━━━━┛
       */
    [_MAIN] = LAYOUT_kc( \
        TAB,  Q,    W,    E,    R,    T,           Y,    U,    I,    O,    P,    BSPC, \
        ALPHA,A,    S,    D,    F,    G,           H,    J,    K,    L,    SCLN, ENT,  \
        LSFT, Z,    X,    C,    V,    B,           N,    M,    COMM, DOT,  SLSH, D_CTRL_EQUAL,\
        _____,_____,_____,LALT, LGUI, SPC,         SPC,  BETA, QUOT, _____,_____,_____ \
    ),

    /* Alpha layer (α)
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┓      ┏━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃     ┃PREV │PLAY │NEXT │     │     ┃      ┃     │ ^^^ │  ^  │ vvv │  ~  ┃ DEL ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨      ┠─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │VOL -│VOL +│     │     ┃      ┃HOME │ <-- │  v  │ --> │  `  ┃  \  ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨      ┠─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │     │     │     │     ┃      ┃ END │ INS │  [  │  ]  │  (  ┃  )  ┃
       ┗━━━━━┻━━━━━┷━━━━━╈━━━━━╈━━━━━╈━━━━━┫      ┠━━━━━╈━━━━━╈━━━━━╈━━━━━┷━━━━━┻━━━━━┛
                         ┃     ┃     ┃     ┃      ┃     ┃     ┃     ┃
                         ┗━━━━━┻━━━━━┻━━━━━┛      ┗━━━━━┻━━━━━┻━━━━━┛
       */
    [_ALPHA] = LAYOUT_kc( \
        _____,MPRV, MPLY, MNXT, _____,_____,       _____,PGUP, UP,   PGDN, TILD, DEL,  \
        _____,_____,VOLD, VOLU, _____,_____,       HOME, LEFT, DOWN, RIGHT,GRV,  BSLS, \
        _____,_____,_____,_____,_____,_____,       END,  INS,  LBRC, RBRC, SH(9),SH(0),\
        _____,_____,_____,_____,_____,_____,       _____,_____,_____,_____,_____,_____ \
    ),

    /* Beta layer (β)
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┓      ┏━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃  `  ┃  1  │  2  │  3  │  4  │  5  ┃      ┃  6  │  7  │  8  │  9  │  0  ┃ del ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨      ┠─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃ F1  │ F2  │ F3  │ F4  │ F5  ┃      ┃ F6  │ F7  │ F8  │ F9  │ F10 ┃  |  ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨      ┠─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃RESET│DEBUG│     │ F11 │ F12 ┃      ┃SND +│SND -│  {  │  }  │  -  ┃  =  ┃
       ┗━━━━━┻━━━━━┷━━━━━╈━━━━━╈━━━━━╈━━━━━┫      ┣━━━━━╈━━━━━╈━━━━━╈━━━━━┷━━━━━┻━━━━━┛
                         ┃     ┃     ┃     ┃      ┃     ┃     ┃     ┃
                         ┗━━━━━┻━━━━━┻━━━━━┛      ┗━━━━━┻━━━━━┻━━━━━┛
       */
    [_BETA] = LAYOUT_kc( \
        _____,K1,   K2,   K3,   K4,   K5,          K6,   K7,   K8,   K9,   K0,   DEL,  \
        _____,F1,   F2,   F3,   F4,   F5,          F6,   F7,   F8,   F9,   F10,  PIPE, \
        _____,RESET,DEBUG,_____,F11,  F12,         AON,  AOFF, LCBR, RCBR, MINS, EQL,  \
        _____,_____,_____,_____,_____,_____,       _____,_____,_____,_____,_____,_____ \
    )
};

void matrix_init_user(void) {
    rgblight_setrgb_at(255, 0, 255, 0);
}

void matrix_slave_scan_user(void) {
    TXLED1;
    _delay_us(10000);
    TXLED0;
    _delay_us(10000);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint16_t alpha_esc_start = 0;
    static uint16_t ctrl_equal_start = 0;
    switch (keycode) {
        case KC_ALPHA:
            if (record->event.pressed) {
                layer_on(_ALPHA);
                alpha_esc_start = timer_read();
            } else {
                layer_off(_ALPHA);
                if (timer_elapsed(alpha_esc_start) < DUAL_HOLD_TIMEOUT) {
                    TAP(KC_ESC);
                }
            }
            return false;
        case KC_BETA:
            if (record->event.pressed) {
                layer_on(_BETA);
            } else {
                layer_off(_BETA);
            }
            return false;
        case KC_D_CTRL_EQUAL:
            if (record->event.pressed) {
                register_code(KC_RCTL);
                ctrl_equal_start = timer_read();
            } else {
                unregister_code(KC_RCTL);
                if (timer_elapsed(ctrl_equal_start) < DUAL_HOLD_TIMEOUT) {
                    TAP(KC_MINS);
                }
            }
            return false;
        case RESET:
            rgblight_setrgb_at(255, 255, 0, 0);
            break;
    }
    return true;
}

uint32_t layer_state_set_user(uint32_t state) {
    uint8_t layer = biton32(state);
    switch (layer) {
        case _MAIN:
            rgblight_setrgb_at(255, 0, 255, 0);
            break;
        case _ALPHA:
            rgblight_setrgb_at(255, 0, 0, 0);
            break;
        case _BETA:
            rgblight_setrgb_at(0, 0, 255, 0);
            break;
    }
    return state;
}
