#ifndef MAP_H
#define MAP_H

#include <Arduboy2.h>
#include "defines.h"
#include <stdint.h>

#define CELL_EMPTY  0
#define CELL_SOLID  1
#define CELL_BRICK  2

class Map
{
public:
  static uint8_t m_width;
  static uint8_t m_height;
  static uint8_t m_cell[MAP_WIDTH_MAX*MAP_HEIGHT_MAX];
  
  static void Init(uint8_t width, uint8_t height);
  static void Draw(int16_t offset_x, int16_t offset_y);
};

#endif
