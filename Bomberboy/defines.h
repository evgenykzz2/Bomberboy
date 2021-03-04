#ifndef DEFINES_H
#define DEFINES_H

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

#endif
