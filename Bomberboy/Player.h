#ifndef PLAYER_H
#define PLAYER_H

#include <Arduboy2.h>
#include "defines.h"
#include <stdint.h>

struct Player
{
  uint8_t x;
  uint8_t y;
  uint8_t movement_frame;
  uint8_t flags;  //000LAPoo    live:1 active:1 orientation:2
  uint8_t lives;
  uint8_t bomb_maximum;
  uint8_t bomb_current;
  uint8_t bomb_radius;

  static void Control(Player* player, uint8_t buttons, uint16_t frame_number);
  static void Draw(Player* player);
};

#endif
