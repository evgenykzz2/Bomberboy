#include "Game.h"
#include "Map.h"

Unit Game::m_units[UNITS_MAX];

void Game::Init()
{
  for (uint8_t i = 0; i < UNITS_MAX; ++i)
    m_units[i].flags = 0;
}

void Game::StartLevel()
{
  Map::Init(15, 7);

  //Clear enemies
  for (uint8_t i = 0; i < UNITS_MAX; ++i)
    m_units[i].flags = 0;

  //Put emenies
  uint8_t enemy_index = 0;
  for (uint8_t tries = 0; tries < 254; tries++)
  {
    uint8_t x = random(Map::m_width);
    uint8_t y = random(Map::m_height);
    if (*(Map::m_cell + y * MAP_WIDTH_MAX + x) != CELL_EMPTY)
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

void Game::Control(uint16_t frame_number)
{
  for (uint8_t i = 0; i < UNITS_MAX; ++i)
  {
    if ((uint8_t)(m_units[i].flags & UNIT_FLAG_ALIVE) != 0)
      Unit::Move(m_units+i, frame_number+i);
  }
}

void Game::Draw(uint16_t frame_number)
{
  Map::Draw(0, 0);
  
  for (uint8_t i = 0; i < UNITS_MAX; ++i)
  {
    if ((uint8_t)(m_units[i].flags & UNIT_FLAG_ACTIVE) != 0)
      Unit::Draw(m_units+i, frame_number+i);
  }
}
