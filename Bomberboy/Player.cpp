#include "Player.h"
#include "Map.h"
#include "Game.h"
#include "assets.h"
#include <Arduboy2.h>

void Player::LevelReset(Player* player)
{
  player->x = STARTUP_CELL_X*8;
  player->y = STARTUP_CELL_Y*8;
  player->flags = UNIT_FLAG_ACTIVE | UNIT_FLAG_ALIVE;
  player->bomb_put = 0;
  player->bomb_last_cell_x = 0;
  player->bomb_last_cell_y = 0;
}

static bool CanPassCell(Player* player, uint8_t cell_x, uint8_t cell_y)
{
  if (cell_x == player->bomb_last_cell_x && cell_y == player->bomb_last_cell_y)
    return true;
  if ( Map::m_cell[cell_x + cell_y*MAP_WIDTH_MAX] == CELL_EMPTY )
    return true;
  return false;
}

static void AllignVertical(Player* player)
{
  uint8_t rest = player->y & 7;
  if (rest <= 2)
    player->y = player->y & (~7);
  else if (rest >= 5)
    player->y = (player->y & (~7)) + 8;
}

static void AllignHorizontal(Player* player)
{
  uint8_t rest = player->x & 7;
  if (rest <= 2)
    player->x = player->x & (~7);
  else if (rest >= 5)
    player->x = (player->x & (~7)) + 8;
}

void Player::Control(Player* player, uint8_t buttons, uint16_t frame_number)
{
  if ( (uint8_t)(frame_number & 3) != 0)
    return;

  //Dead can't control unit
  if ( (uint8_t)(player->flags & UNIT_FLAG_ALIVE) == 0)
    return;
  
  if (buttons == 0)
    return;

  if ( (uint8_t)(buttons & A_BUTTON) != 0 && player->bomb_put < player->bomb_maximum )
  {
    uint8_t orientation = player->flags & ORIENTATION_MASK;  
    uint8_t cell_x = (player->x + 4) / 8;
    uint8_t cell_y = (player->y + 4) / 8;
    if ( *(Map::m_cell + cell_y * MAP_WIDTH_MAX + cell_x) == CELL_EMPTY )
    {
      uint8_t index = Map::FindUnusedBombIndex();
      if (index != 0xFF)
      {
        *(Map::m_cell + cell_y * MAP_WIDTH_MAX + cell_x) = CELL_BOMB_INITIAL;
        player->bomb_put ++;
        player->bomb_last_cell_x = cell_x;
        player->bomb_last_cell_y = cell_y;
        Map::m_bombs[index].cell_x = cell_x;
        Map::m_bombs[index].cell_y = cell_y;
        Map::m_bombs[index].radius = player->bomb_radius;
        Map::m_bombs[index].activation_time = BOMBS_ACTIVATION_FRAMES;
        Map::m_bombs[index].radius_counter = 0;
        Map::m_bombs[index].flags = BOMB_FLAG_LEFT | BOMB_FLAG_RIGHT | BOMB_FLAG_UP | BOMB_FLAG_DOWN;
      }
    }
  }

  if ( (uint8_t)(buttons & (LEFT_BUTTON | RIGHT_BUTTON)) != 0)  //Do vertical align
    AllignVertical(player);

  if ( (uint8_t)(buttons & (UP_BUTTON | DOWN_BUTTON)) != 0) //Do horizontal align
    AllignHorizontal(player);

  //Movements
  if (player->bomb_last_cell_x != 0)
  {
    //control last bomb position
    uint8_t cell_x = (player->x + 4) / 8;
    uint8_t cell_y = (player->y + 4) / 8;
    if (cell_x != player->bomb_last_cell_x || cell_y != player->bomb_last_cell_y)
    {
      player->bomb_last_cell_x = 0;
      player->bomb_last_cell_y = 0;
    }
  }
  
  if ((uint8_t)(buttons & LEFT_BUTTON) != 0)
  {
    player->flags = (player->flags & (~ORIENTATION_MASK)) | ORIENTATION_LEFT;
    uint8_t cell_x = (player->x - 1)/8;
    uint8_t cell_y0 = player->y/8;
    uint8_t cell_y1 = (player->y + 7)/8;
    if (CanPassCell(player, cell_x, cell_y0) && CanPassCell(player, cell_x, cell_y1))
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
    if (CanPassCell(player, cell_x, cell_y0) && CanPassCell(player, cell_x, cell_y1))
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
    if (CanPassCell(player, cell_x0, cell_y) && CanPassCell(player, cell_x1, cell_y))
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
    if (CanPassCell(player, cell_x0, cell_y) && CanPassCell(player, cell_x1, cell_y))
    {
      player->movement_frame++;
      player->y ++;
    }
  }
}

void Player::Draw(Player* player)
{
  uint8_t f = player->movement_frame & 3;
  //f += 12;  //Die
  uint8_t orientation = player->flags & ORIENTATION_MASK;
  if (orientation == ORIENTATION_RIGHT)
    f += 4;
  if (orientation == ORIENTATION_DOWN || orientation == ORIENTATION_UP)
    f += 8;
  Sprites::drawPlusMask(player->x+Game::m_draw_offset_x, player->y+Game::m_draw_offset_y, s_sprites+f*18, 0);
}
