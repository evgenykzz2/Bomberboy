#ifndef MAP_H
#define MAP_H

#include <Arduboy2.h>
#include "defines.h"
#include <stdint.h>

#define CELL_EMPTY  0
#define CELL_SOLID  1
#define CELL_BRICK  2
#define CELL_BOMB_INITIAL 0x08
#define CELL_BOMB_EXPLOSION       0x10
#define CELL_BOMB_EXPLOSION_HORIZONTAL  0x18
#define CELL_BOMB_EXPLOSION_VERTICAL    0x20

#define BOMB_FLAG_LEFT  1
#define BOMB_FLAG_RIGHT 2
#define BOMB_FLAG_UP    4
#define BOMB_FLAG_DOWN  8

struct Bomb
{
  uint8_t cell_x;
  uint8_t cell_y;
  uint8_t activation_time;
  uint8_t radius;
  uint8_t radius_counter;
  uint8_t flags;
};

class Map
{
public:
  static uint8_t m_width;
  static uint8_t m_height;
  static uint8_t m_cell[MAP_WIDTH_MAX*MAP_HEIGHT_MAX];
  
  static Bomb m_bombs[BOMBS_MAX];
  static uint8_t FindUnusedBombIndex();
  
  static void Init(uint8_t width, uint8_t height);
  static void Control(uint16_t frame_number);
  static void Draw();
};

#endif
