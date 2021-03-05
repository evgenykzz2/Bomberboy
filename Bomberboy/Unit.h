#ifndef UNIT_H
#define UNIT_H

#include "defines.h"
#include <stdint.h>

#define UNIT_SPRITE_DIE 24

struct Unit
{
  uint8_t x;
  uint8_t y;
  uint8_t flags;  //000LAPoo    live:1 active:1 orientation:2
  uint8_t type;   //

  static int8_t s_orientation_dx[4];
  static int8_t s_orientation_dy[4];

  static int8_t s_forward_dx[4];
  static int8_t s_forward_dy[4];

  static void Control(Unit* unit, uint16_t frame_number);
  static void Draw(Unit* unit, uint16_t frame_number);
};

#endif
