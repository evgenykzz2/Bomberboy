#include "Map.h"
#include "assets.h"

uint8_t Map::m_cell[MAP_WIDTH_MAX*MAP_HEIGHT_MAX];
uint8_t Map::m_width = 1;
uint8_t Map::m_height = 1;


void Map::Init(uint8_t width, uint8_t height)
{
  m_width = width;
  m_height = height;
  for (uint8_t y = 0; y < height; ++y)
  {
    uint8_t* line = m_cell + y * MAP_WIDTH_MAX;
    for (uint8_t x = 0; x < width; ++x)
    {
      if (x == 0 || x+1 == width || y == 0 || y+1 == height)
        line[x] = CELL_SOLID;
      else if ( uint8_t(x & 1) == 0 && uint8_t(y & 1) == 0)
        line[x] = CELL_SOLID;
      else
      {
        if (random(100) < 10)
          line[x] = CELL_BRICK;
        else
          line[x] = CELL_EMPTY;
      }
    }
  }
}

void Map::Draw(int16_t offset_x, int16_t offset_y)
{
  for (uint8_t y = 0; y < m_height; ++y)
  {
    uint8_t* line = m_cell + y * MAP_WIDTH_MAX;
    for (uint8_t x = 0; x < m_width; ++x)
    {
      Arduboy2::drawBitmap(x*8, y*8, s_tiles + ((uint16_t)line[x])*8, 8, 8);
    }
  }
}
