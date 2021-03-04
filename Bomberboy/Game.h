#ifndef GAME_H
#define GAME_H

#include "Unit.h"

class Game
{
public:
  static Unit m_units[UNITS_MAX];

  static void Init();

  static void StartLevel();

  static void Control(uint16_t frame_number);
  static void Draw(uint16_t frame_number);
};

#endif
