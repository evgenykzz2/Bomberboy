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
  static uint8_t m_level;
  static uint8_t m_splash_level;
  static uint8_t m_pause_toggle;
  static uint8_t m_pause_mode;
  static uint8_t m_ghost_left;

  static void Init();

  static void StartLevel();
  
  static bool Control(uint8_t buttons, uint16_t frame_number);
  static void Draw(Arduboy2& arduboy);
};

#endif
