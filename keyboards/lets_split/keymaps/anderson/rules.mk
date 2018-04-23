ifndef QUANTUM_DIR
	include ../../../../Makefile
endif

RGBLIGHT_ENABLE = yes
POINTING_DEVICE_ENABLE = yes
KNOB_ENABLE = yes
SRC += solid.c dmc12.c rainbow.c knob_v2.c
