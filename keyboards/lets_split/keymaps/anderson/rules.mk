ifndef QUANTUM_DIR
	include ../../../../Makefile
endif

RGBLIGHT_ENABLE = yes
POINTING_DEVICE_ENABLE = yes
SRC += solid.c dmc12.c rainbow.c knob.c
