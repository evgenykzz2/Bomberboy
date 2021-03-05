#ifndef GAME_H
#define GAME_H

#include "Unit.h"
#include "Player.h"

class Game
{
public:
  static Unit m_units[UNITS_MAX];
  static Player m_player;
  static int16_t m_draw_offset_x;
  static int16_t m_draw_offset_y;

  static void Init();

  static void StartLevel();

  static void Control(uint8_t buttons, uint16_t frame_number);
  static void Draw(uint16_t frame_number);
};

#endif
