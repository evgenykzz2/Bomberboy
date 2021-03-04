#include "Player.h"
#include "Map.h"
#include "assets.h"
#include <Arduboy2.h>

void Player::Control(Player* player, uint8_t buttons, uint16_t frame_number)
{
  if (frame_number & 3 != 0)
    return;
  if (buttons == 0)
    return;

  if ( (uint8_t)(buttons & (LEFT_BUTTON | RIGHT_BUTTON)) != 0)
  {
    //Do vertical align
    uint8_t rest = player->y & 7;
    if (rest <= 2)
      player->y = player->y & (~7);
    else if (rest >= 5)
      player->y = (player->y & (~7)) + 8;
  }

  if ( (uint8_t)(buttons & (UP_BUTTON | DOWN_BUTTON)) != 0)
  {
    //Do horizontal align
    uint8_t rest = player->x & 7;
    if (rest <= 2)
      player->x = player->x & (~7);
    else if (rest >= 5)
      player->x = (player->x & (~7)) + 8;
  }

  //Mmovements
  if ((uint8_t)(buttons & LEFT_BUTTON) != 0)
  {
    player->flags = (player->flags & (~ORIENTATION_MASK)) | ORIENTATION_LEFT;
    uint8_t cell_x = (player->x - 1)/8;
    uint8_t cell_y0 = player->y/8;
    uint8_t cell_y1 = (player->y + 7)/8;
    if (Map::m_cell[cell_x + cell_y0*MAP_WIDTH_MAX] == CELL_EMPTY && Map::m_cell[cell_x + cell_y1*MAP_WIDTH_MAX] == CELL_EMPTY)
    {
      player->movement_frame++;
      player->x --;
    }
  }

  if ((uint8_t)(buttons & RIGHT_BUTTON) != 0)
  {
    player->flags = (player->flags & (~ORIENTATION_MASK)) | ORIENTATION_RIGHT;
    uint8_t cell_x = (player->x + 8)/8;
    uint8_t cell_y0 = player->y/8;
    uint8_t cell_y1 = (player->y + 7)/8;
    if (Map::m_cell[cell_x + cell_y0*MAP_WIDTH_MAX] == CELL_EMPTY && Map::m_cell[cell_x + cell_y1*MAP_WIDTH_MAX] == CELL_EMPTY)
    {
      player->movement_frame++;
      player->x ++;
    }
  }

  if ((uint8_t)(buttons & UP_BUTTON) != 0)
  {
    player->flags = (player->flags & (~ORIENTATION_MASK)) | ORIENTATION_UP;
    uint8_t cell_y = (player->y - 1)/8;
    uint8_t cell_x0 = player->x/8;
    uint8_t cell_x1 = (player->x + 7)/8;
    if (Map::m_cell[cell_x0 + cell_y*MAP_WIDTH_MAX] == CELL_EMPTY && Map::m_cell[cell_x1 + cell_y*MAP_WIDTH_MAX] == CELL_EMPTY)
    {
      player->movement_frame++;
      player->y --;
    }
  }

  if ((uint8_t)(buttons & DOWN_BUTTON) != 0)
  {
    player->flags = (player->flags & (~ORIENTATION_MASK)) | ORIENTATION_DOWN;
    uint8_t cell_y = (player->y + 8)/8;
    uint8_t cell_x0 = player->x/8;
    uint8_t cell_x1 = (player->x + 7)/8;
    if (Map::m_cell[cell_x0 + cell_y*MAP_WIDTH_MAX] == CELL_EMPTY && Map::m_cell[cell_x1 + cell_y*MAP_WIDTH_MAX] == CELL_EMPTY)
    {
      player->movement_frame++;
      player->y ++;
    }
  }
}

void Player::Draw(Player* player)
{
  uint8_t f = player->movement_frame & 3;
  uint8_t orientation = player->flags & ORIENTATION_MASK;
  if (orientation == ORIENTATION_RIGHT)
    f += 4;
  if (orientation == ORIENTATION_DOWN || orientation == ORIENTATION_UP)
    f += 8;
  Sprites::drawPlusMask(player->x, player->y, s_sprites+f*18, 0);
}
