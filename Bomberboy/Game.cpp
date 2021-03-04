#include "Game.h"
#include "Map.h"

Unit Game::m_units[UNITS_MAX];
Player Game::m_player;

void Game::Init()
{
  //Clear player
  m_player.flags = 0;
  m_player.lives = 3;
  m_player.bomb_maximum = 1;
  m_player.bomb_current = 0;
  m_player.bomb_radius = 1;
}

void Game::StartLevel()
{
  Map::Init(15, 7);

  //Prepare player
  m_player.x = STARTUP_CELL_X*8;
  m_player.y = STARTUP_CELL_Y*8;
  m_player.flags = UNIT_FLAG_ACTIVE | UNIT_FLAG_ALIVE;

  *(Map::m_cell + STARTUP_CELL_Y * MAP_WIDTH_MAX + STARTUP_CELL_X) = CELL_EMPTY;  //Get some free space
  
  //Clear enemies
  for (uint8_t i = 0; i < UNITS_MAX; ++i)
    m_units[i].flags = 0;

  //Put emenies
  uint8_t enemy_index = 0;
  for (uint8_t tries = 0; tries < 254; tries++)
  {
    uint8_t x = random(Map::m_width);
    uint8_t y = random(Map::m_height);
    //Cell is busy?
    if (*(Map::m_cell + y * MAP_WIDTH_MAX + x) != CELL_EMPTY)
      continue;
    //Too close to the player?
    if (abs(x - STARTUP_CELL_X) <= 2 && abs(y - STARTUP_CELL_Y) <= 2)
      continue;
    m_units[enemy_index].flags =
              (random(4) & ORIENTATION_MASK) //Orientation
              | UNIT_FLAG_ACTIVE | UNIT_FLAG_ALIVE;
    m_units[enemy_index].x = x*8;
    m_units[enemy_index].y = y*8;
    m_units[enemy_index].type = 0;
    enemy_index ++;
    if (enemy_index >= 2)
      break;
    if (enemy_index >= UNITS_MAX)
      break;
  }
}

void Game::Control(uint8_t buttons, uint16_t frame_number)
{
  Player::Control(&m_player, buttons, frame_number);
  
  for (uint8_t i = 0; i < UNITS_MAX; ++i)
  {
    if ((uint8_t)(m_units[i].flags & UNIT_FLAG_ALIVE) != 0)
      Unit::Move(m_units+i, frame_number+i);
  }
}

void Game::Draw(uint16_t frame_number)
{
  Map::Draw(0, 0);

  Player::Draw(&m_player);
  for (uint8_t i = 0; i < UNITS_MAX; ++i)
  {
    if ((uint8_t)(m_units[i].flags & UNIT_FLAG_ACTIVE) != 0)
      Unit::Draw(m_units+i, frame_number+i);
  }
}
