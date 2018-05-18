#ifndef CONFIG_USER_H
#define CONFIG_USER_H

#include "../../config.h"

// Key matrix pins
#undef MATRIX_COL_PINS
#undef MATRIX_ROW_PINS
#define MATRIX_ROW_PINS { D7, E6, B4, B5 }
#define MATRIX_COL_PINS { B1, B3, B2, B6 }

// Number of backlight levels */
#undef BACKLIGHT_PIN
#define BACKLIGHT_PIN D4

// Mousekeys
#define MOUSEKEY_DELAY 0
#define MOUSEKEY_INTERVAL 40

#endif
