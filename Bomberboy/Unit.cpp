#include "Unit.h"
#include "Map.h"
#include "Game.h"
#include "assets.h"
#include <Arduboy2.h>

//                                  L   R    U   D
int8_t Unit::s_orientation_dx[4] = {-1, +1,  0,  0};
int8_t Unit::s_orientation_dy[4] = { 0,  0, -1, +1};
int8_t Unit::s_forward_dx[4] =     {-1, +8,  0,  0};
int8_t Unit::s_forward_dy[4]=      { 0,  0, -1, +8};
  
void Unit::Move(Unit* unit, uint16_t frame_number)
{
  if ((uint8_t)(frame_number & 3) != 0)
    return;
  uint8_t orientation = unit->flags & ORIENTATION_MASK;
  int16_t forward_x = (int16_t)unit->x + s_forward_dx[orientation];
  int16_t forward_y = (int16_t)unit->y + s_forward_dy[orientation];

  uint8_t cell_x = forward_x/8;
  uint8_t cell_y = forward_y/8;
  uint8_t cell = Map::m_cell[cell_x + cell_y*MAP_WIDTH_MAX];
  if (cell == CELL_EMPTY)
  {
    unit->x += s_orientation_dx[orientation];
    unit->y += s_orientation_dy[orientation];
  } else
    unit->flags = (unit->flags & (~ORIENTATION_MASK)) | (random(4) & ORIENTATION_MASK);
}

void Unit::Draw(Unit* unit, uint16_t frame_number)
{
  uint8_t f = (frame_number / 4) & 3;
  uint8_t orientation = unit->flags & ORIENTATION_MASK;
  if (orientation == ORIENTATION_RIGHT || orientation == ORIENTATION_DOWN)
    f += 4;
  Sprites::drawPlusMask(unit->x+Game::m_draw_offset_x, unit->y+Game::m_draw_offset_y, s_sprites+(f+16)*18, 0);
}
