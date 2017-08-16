#include "lets_split.h"
#include "action_layer.h"
#include "eeconfig.h"

extern keymap_config_t keymap_config;

// Command / brace setup
#ifndef LCBO_KEY // left command / brace open {
  #define LCBO_KEY KC_LBRACKET
#endif
#ifndef RCBC_KEY // right command / brace close }
  #define RCBC_KEY KC_RBRACKET
#endif

static bool command_interrupted[2] = {0, 0};
static uint16_t space_cadet_command_timer[2] = {0, 0};

// option / angle setup
#ifndef LOAO_KEY // left option / angle open <
  #define LOAO_KEY KC_COMM
#endif
#ifndef ROAC_KEY // right option / angle close >
  #define ROAC_KEY KC_DOT
#endif

static bool option_interrupted[2] = {0, 0};
static uint16_t space_cadet_option_timer[2] = {0, 0};

static uint16_t space_cadet_control_timer = 0;

#define _QWERTY 0
#define _LOWER 1
#define _RAISE 2

enum custom_keycodes {
  KC_LCBO = SAFE_RANGE, // left command brace open {
  KC_RCBC, // right command brace close }

  KC_LOAO, // left option angle open <
  KC_ROAC, // right option angle close >

  KC_CTLDQ, // left control double quote "
};

#define KC_ KC_TRNS
#define _______ KC_TRNS

#define KC_CAPW LGUI(LSFT(KC_3))        // Capture whole screen
#define KC_CPYW LGUI(LSFT(LCTL(KC_3)))  // Copy whole screen
#define KC_CAPP LGUI(LSFT(KC_4))        // Capture portion of screen
#define KC_CPYP LGUI(LSFT(LCTL(KC_4)))  // Copy portion of screen
#define KC_X0 KC_CTLDQ
#define KC_X1 LT(_LOWER, KC_LBRC)
#define KC_X2 LT(_RAISE, KC_RBRC)
#define KC_X3 LT(_FN3, KC_GRV)
#define KC_X4 MT(MOD_LSFT, KC_ENT)
#define KC_HYPE ALL_T(KC_ESC)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QWERTY] = KC_KEYMAP(
  //,----+----+----+----+----+----.    ,----+----+----+----+----+----.
     TAB , Q  , W  , E  , R  , T  ,      Y  , U  , I  , O  , P  ,EQL,
  //|----+----+----+----+----+----|    |----+----+----+----+----+----|
      X0 , A  , S  , D  , F  , G  ,      H  , J  , K  , L  ,SCLN, ENT,
  //|----+----+----+----+----+----|    |----+----+----+----+----+----|
     LSPO, Z  , X  , C  , V  , B  ,      N  , M  ,COMM,DOT ,SLSH,RSPC,
  //|----+----+----+----+----+----|    |----+----+----+----+----+----|
      GRV,HYPE, X1 ,LOAO,LCBO,BSPC,     SPC, RCBC,ROAC, X2, AMPR,QUES
  //`----+----+----+----+----+----'    `----+----+----+----+----+----'
  ),

  [_LOWER] = KC_KEYMAP(
  //,----+----+----+----+----+----.    ,----+----+----+----+----+----.
         ,    ,    ,    ,    ,    ,     CIRC, AT ,HASH, DLR,PERC,BSLS,
  //|----+----+----+----+----+----|    |----+----+----+----+----+----|
         ,    ,    ,    ,    ,    ,     LEFT,DOWN, UP ,RGHT,QUOT,PIPE,
  //|----+----+----+----+----+----|    |----+----+----+----+----+----|
         ,    ,    ,    ,    ,    ,     0   ,AMPR,ASTR,EXLM, GRV,TILD,
  //|----+----+----+----+----+----|    |----+----+----+----+----+----|
         ,    ,    ,    ,    ,    ,         ,    ,    ,    ,    ,
  //`----+----+----+----+----+----'    `----+----+----+----+----+----'
  ),

  [_RAISE] = KC_KEYMAP(
  //,----+----+----+----+----+----.    ,----+----+----+----+----+----.
         ,EQL ,PLUS, 7  , 8  , 9  ,         ,    ,    ,    ,    ,    ,
  //|----+----+----+----+----+----|    |----+----+----+----+----+----|
         ,UNDS,MINS, 4  , 5  , 6  ,         ,    ,    ,    ,    ,    ,
  //|----+----+----+----+----+----|    |----+----+----+----+----+----|
     MUTE,CAPP,ASTR, 1  , 2  , 3  ,         ,    ,    ,    ,    ,    ,
  //|----+----+----+----+----+----|    |----+----+----+----+----+----|
         ,    ,    ,TAB , 0  ,ENT ,         ,    ,    ,    ,    ,
  //`----+----+----+----+----+----'    `----+----+----+----+----+----'
  )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_LCBO: {
      if (record->event.pressed) {
        command_interrupted[0] = false;
        space_cadet_command_timer[0] = timer_read ();
        register_mods(MOD_BIT(KC_LGUI));
      }
      else {
        unregister_mods(MOD_BIT(KC_LGUI));
        #ifdef DISABLE_SPACE_CADET_ROLLOVER
          if (get_mods() & MOD_BIT(KC_RGUI)) { // is opposite side command down?
            command_interrupted[0] = true;
            command_interrupted[1] = true;
          }
        #endif
        if (!command_interrupted[0] && timer_elapsed(space_cadet_command_timer[0]) < TAPPING_TERM) {
          register_mods(MOD_BIT(KC_LSFT));
          register_code(LCBO_KEY);
          unregister_code(LCBO_KEY);
          unregister_mods(MOD_BIT(KC_LSFT));
        }
      }
      return false;
    }

    case KC_RCBC: {
      if (record->event.pressed) {
        command_interrupted[1] = false;
        space_cadet_command_timer[1] = timer_read ();
        register_mods(MOD_BIT(KC_RGUI));
      }
      else {
        unregister_mods(MOD_BIT(KC_RGUI));
        #ifdef DISABLE_SPACE_CADET_ROLLOVER
          if (get_mods() & MOD_BIT(KC_LGUI)) { // is opposite side command down?
            command_interrupted[0] = true;
            command_interrupted[1] = true;
          }
        #endif
        if (!command_interrupted[1] && timer_elapsed(space_cadet_command_timer[1]) < TAPPING_TERM) {
          register_mods(MOD_BIT(KC_RSFT));
          register_code(RCBC_KEY);
          unregister_code(RCBC_KEY);
          unregister_mods(MOD_BIT(KC_RSFT));
        }
      }
      return false;
    }

    case KC_LOAO: {
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
          register_mods(MOD_BIT(KC_LSFT));
          register_code(LOAO_KEY);
          unregister_code(LOAO_KEY);
          unregister_mods(MOD_BIT(KC_LSFT));
        }
      }
      return false;
    }

    case KC_ROAC: {
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
          register_code(ROAC_KEY);
          unregister_code(ROAC_KEY);
          unregister_mods(MOD_BIT(KC_RSFT));
        }
      }
      return false;
    }

    case KC_CTLDQ: {
      if (record->event.pressed) {
        space_cadet_control_timer = timer_read ();
        register_mods(MOD_BIT(KC_LCTL));
      }
      else {
        unregister_mods(MOD_BIT(KC_LCTL));
        if (timer_elapsed(space_cadet_control_timer) < TAPPING_TERM) {
          register_mods(MOD_BIT(KC_RSFT));
          register_code(KC_QUOT);
          unregister_code(KC_QUOT);
          unregister_mods(MOD_BIT(KC_RSFT));
        }
      }
      return false;
    }
  }
  return true;
}
