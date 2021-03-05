#ifndef DEFINES_H
#define DEFINES_H

#include <Arduboy2.h>

#define TARGET_FRAMERATE 30

#define MAP_WIDTH_MAX  32
#define MAP_HEIGHT_MAX 16

#define ORIENTATION_MASK  0x03
#define ORIENTATION_LEFT  0x00
#define ORIENTATION_RIGHT 0x01
#define ORIENTATION_UP    0x02
#define ORIENTATION_DOWN  0x03

#define UNIT_FLAG_PASS_BRICK   0x04
#define UNIT_FLAG_ACTIVE       0x08
#define UNIT_FLAG_ALIVE        0x10

#define UNITS_MAX 16
#define BOMBS_MAX 16
#define BOMBS_ACTIVATION_FRAMES 150

#define STARTUP_CELL_X 1
#define STARTUP_CELL_Y 1

#define LEVEL_TITLE_DURATION_FRAMES 60
#define PAUSE_PRESS_DURATION 60
#define INVULNERABILITY 150

#endif
