#ifndef MENU_H
#define MENU_H

#include "defines.h"
#include <stdint.h>

class Menu
{
public:
  static int16_t m_bomb_speed;
  static int16_t m_bomb_pos;
  static uint8_t m_logo_pos;

  static void Init();
  static bool Control(uint8_t buttons, uint16_t frame_number);
  static void Draw(Arduboy2& arduboy);
};

#endif
