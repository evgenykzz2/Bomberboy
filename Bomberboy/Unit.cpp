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
  
void Unit::Control(Unit* unit, uint16_t frame_number)
{
  //Die test
  if ( (uint8_t)(unit->flags & UNIT_FLAG_ALIVE) != 0 )
  {
    uint8_t cell_x0 = unit->x / 8;
    uint8_t cell_x1 = (unit->x+7) / 8;
    uint8_t cell_y0 = unit->y / 8;
    uint8_t cell_y1 = (unit->y+7) / 8;
    if ( (Map::m_cell[cell_x0 + cell_y0*MAP_WIDTH_MAX] >= CELL_BOMB_EXPLOSION && Map::m_cell[cell_x0 + cell_y0*MAP_WIDTH_MAX] <= CELL_BOMB_EXPLOSION_LAST) ||
         (Map::m_cell[cell_x1 + cell_y0*MAP_WIDTH_MAX] >= CELL_BOMB_EXPLOSION && Map::m_cell[cell_x1 + cell_y0*MAP_WIDTH_MAX] <= CELL_BOMB_EXPLOSION_LAST) ||
         (Map::m_cell[cell_x0 + cell_y1*MAP_WIDTH_MAX] >= CELL_BOMB_EXPLOSION && Map::m_cell[cell_x0 + cell_y1*MAP_WIDTH_MAX] <= CELL_BOMB_EXPLOSION_LAST) ||
         (Map::m_cell[cell_x1 + cell_y1*MAP_WIDTH_MAX] >= CELL_BOMB_EXPLOSION && Map::m_cell[cell_x1 + cell_y1*MAP_WIDTH_MAX] <= CELL_BOMB_EXPLOSION_LAST) )
    {
      unit->flags = unit->flags & (~UNIT_FLAG_ALIVE);
      Game::m_ghost_left --;
      unit->type = 0; //Use type as frame counter
      return;
    }
  } else
  {
    unit->type ++;
    if (unit->type >= 8*4)
      unit->flags = 0;
    return;
  }

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
  if ( (uint8_t)(unit->flags & UNIT_FLAG_ALIVE) != 0 )
  {
    uint8_t f = (frame_number / 4) & 3;
    uint8_t orientation = unit->flags & ORIENTATION_MASK;
    if (orientation == ORIENTATION_RIGHT || orientation == ORIENTATION_DOWN)
      f += 4;
    Sprites::drawPlusMask(unit->x+Game::m_draw_offset_x, unit->y+Game::m_draw_offset_y, s_sprites+(f+16)*18, 0);
  } else
  {
    uint16_t f = UNIT_SPRITE_DIE + (unit->type / 4);
    Sprites::drawPlusMask(unit->x+Game::m_draw_offset_x, unit->y+Game::m_draw_offset_y, s_sprites+f*18, 0);
  }
}
