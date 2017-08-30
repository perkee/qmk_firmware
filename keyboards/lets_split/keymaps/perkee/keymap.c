#include "lets_split.h"
#include "action_layer.h"
#include "eeconfig.h"

extern keymap_config_t keymap_config;

static uint16_t space_cadet_hyper_timer[2] = {0, 0};

static bool option_interrupted[2] = {0, 0};
static uint16_t space_cadet_option_timer[2] = {0, 0};

static uint16_t space_cadet_control_timer = 0;

#define _QWERTY 0
#define _LOWER 1
#define _RAISE 2

enum custom_keycodes {
  KC_LHAO  // left hyper angle open <
    = SAFE_RANGE,
  KC_RHAC, // right hyper angle close >

  KC_LOBO, // left option brace open {
  KC_ROBC, // right option brace close }

  KC_LCDQ // left control double quote "
};

#define KC_ KC_TRNS

#define KC_CAPW LGUI(LSFT(KC_3))        // Capture whole screen
#define KC_CAPP LGUI(LSFT(KC_4))        // Capture portion of screen

#define KC_RSET RESET
#define KC_RC_ENT MT(MOD_RCTL, KC_ENT)

#define KC_LCBS MT(MOD_LGUI, KC_BSPC) // left command backspace
#define KC_RCSP MT(MOD_RGUI, KC_SPC)  // right command space

#define KC_CMD0 LGUI(KC_0)
#define KC_CMD1 LGUI(KC_1)
#define KC_CMD2 LGUI(KC_2)
#define KC_CMD3 LGUI(KC_3)
#define KC_CMD4 LGUI(KC_4)
#define KC_CMD5 LGUI(KC_5)
#define KC_CMD6 LGUI(KC_6)
#define KC_CMD7 LGUI(KC_7)
#define KC_CMD8 LGUI(KC_8)
#define KC_CMD9 LGUI(KC_9)

#define KC_LO_J LT(_LOWER, KC_J)
#define KC_RS_F LT(_RAISE, KC_F)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QWERTY] = KC_KEYMAP(
  //,----+----+----+----+----+----.    ,----+----+----+----+----+----.
     TAB , Q  , W  , E  , R  , T  ,      Y  , U  , I  , O  , P  , EQL,
  //|----+----+----+----+----+----|    |----+----+----+----+----+----|
     LCDQ, A  , S  , D  ,RS_F, G  ,      H  ,LO_J, K  , L  ,SCLN,RC_ENT,
  //|----+----+----+----+----+----|    |----+----+----+----+----+----|
     LSPO, Z  , X  , C  , V  , B  ,      N  , M  ,COMM,DOT ,SLSH,RSPC,
  //|----+----+----+----+----+----|    |----+----+----+----+----+----|
     GRV , ESC,LBRC,LHAO,LOBO,LCBS,     RCSP,ROBC,RHAC,RBRC,MINS,QUOT
  //`----+----+----+----+----+----'    `----+----+----+----+----+----'
  ),

  [_LOWER] = KC_KEYMAP(
  //,----+----+----+----+----+----.    ,----+----+----+----+----+----.
     CAPW,UNDS, 7  , 8  , 9  , EQL,         ,    ,    ,    ,    ,RSET,
  //|----+----+----+----+----|----+    |----+----+----+----+----+----|
     CAPP,MINS, 4  , 5  , 6  ,PLUS,         ,    ,    ,    ,    ,    ,
  //|----+----+----+----+----|----+    |----+----+----+----+----+----|
     MUTE,ASTR, 1  , 2  , 3  ,SLSH,         ,    ,    ,    ,    ,    ,
  //|----+----+----+----+----|----+    |----+----+----+----+----+----|
     VOLD,VOLU,TAB , 0  ,ENT ,    ,         ,    ,    ,    ,    ,
  //`----+----+----+----+----+----'    `----+----+----+----+----+----'
  ),

  [_RAISE] = KC_KEYMAP(
  //,----+----+----+----+----+----.    ,----+----+----+----+----+----.
         ,    ,    ,    ,    ,    ,     CIRC, AT ,HASH, DLR,PERC,BSLS,
  //|----+----+----+----+----+----|    |----+----+----+----+----+----|
     CAPS,    ,    ,    ,    ,    ,     LEFT,DOWN, UP ,RGHT,QUOT,PIPE,
  //|----+----+----+----+----+----|    |----+----+----+----+----+----|
         ,    ,    ,    ,    ,    ,     0   ,AMPR,ASTR,EXLM, GRV,TILD,
  //|----+----+----+----+----+----|    |----+----+----+----+----+----|
         ,    ,    ,    ,    ,    ,         ,    ,    ,    ,    ,
  //`----+----+----+----+----+----'    `----+----+----+----+----+----'
  )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_LHAO: {
      if (record->event.pressed) {
        space_cadet_hyper_timer[0] = timer_read ();
        register_mods(MOD_BIT(KC_LSHIFT));
        register_mods(MOD_BIT(KC_LGUI));
        register_mods(MOD_BIT(KC_LALT));
        register_mods(MOD_BIT(KC_LCTRL));
      }
      else {
        unregister_mods(MOD_BIT(KC_LCTRL));
        unregister_mods(MOD_BIT(KC_LALT));
        unregister_mods(MOD_BIT(KC_LGUI));
        if (timer_elapsed(space_cadet_hyper_timer[0]) < TAPPING_TERM) {
          register_code(KC_COMMA);
          unregister_code(KC_COMMA);
        }
        unregister_mods(MOD_BIT(KC_LSHIFT));
      }
      return false;
    }
    case KC_RHAC: {
      if (record->event.pressed) {
        space_cadet_hyper_timer[1] = timer_read ();
        register_mods(MOD_BIT(KC_RSHIFT));
        register_mods(MOD_BIT(KC_RGUI));
        register_mods(MOD_BIT(KC_RALT));
        register_mods(MOD_BIT(KC_RCTRL));
      }
      else {
        unregister_mods(MOD_BIT(KC_RCTRL));
        unregister_mods(MOD_BIT(KC_RALT));
        unregister_mods(MOD_BIT(KC_RGUI));
        if (timer_elapsed(space_cadet_hyper_timer[1]) < TAPPING_TERM) {
          register_code(KC_DOT);
          unregister_code(KC_DOT);
        }
        unregister_mods(MOD_BIT(KC_RSHIFT));
      }
      return false;
    }

    case KC_LOBO: {
      if (record->event.pressed) {
        option_interrupted[0] = false;
        space_cadet_option_timer[0] = timer_read ();
        register_mods(MOD_BIT(KC_LALT));
      }
      else {
        unregister_mods(MOD_BIT(KC_LALT));
        #ifdef DISABLE_SPACE_CADET_ROLLOVER
          if (get_mods() & MOD_BIT(KC_RALT)) { // is opposite side option down?
            option_interrupted[0] = true;
            option_interrupted[1] = true;
          }
        #endif
        if (!option_interrupted[0] && timer_elapsed(space_cadet_option_timer[0]) < TAPPING_TERM) {
          register_mods(MOD_BIT(KC_LSHIFT));
          register_code(KC_LBRACKET);
          unregister_code(KC_LBRACKET);
          unregister_mods(MOD_BIT(KC_LSHIFT));
        }
      }
      return false;
    }

    case KC_ROBC: {
      if (record->event.pressed) {
        option_interrupted[1] = false;
        space_cadet_option_timer[1] = timer_read ();
        register_mods(MOD_BIT(KC_RALT));
      }
      else {
        unregister_mods(MOD_BIT(KC_RALT));
        #ifdef DISABLE_SPACE_CADET_ROLLOVER
          if (get_mods() & MOD_BIT(KC_LALT)) { // is opposite side option down?
            option_interrupted[0] = true;
            option_interrupted[1] = true;
          }
        #endif
        if (!option_interrupted[1] && timer_elapsed(space_cadet_option_timer[1]) < TAPPING_TERM) {
          register_mods(MOD_BIT(KC_RSFT));
          register_code(KC_RBRACKET);
          unregister_code(KC_RBRACKET);
          unregister_mods(MOD_BIT(KC_RSFT));
        }
      }
      return false;
    }

    case KC_LCDQ: {
      if (record->event.pressed) {
        space_cadet_control_timer = timer_read ();
        register_mods(MOD_BIT(KC_LCTL));
      }
      else {
        unregister_mods(MOD_BIT(KC_LCTL));
        if (timer_elapsed(space_cadet_control_timer) < TAPPING_TERM) {
          register_mods(MOD_BIT(KC_LSFT));
          register_code(KC_QUOT);
          unregister_code(KC_QUOT);
          unregister_mods(MOD_BIT(KC_LSFT));
        }
      }
      return false;
    }
  }
  return true;
}
