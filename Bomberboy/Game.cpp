#include "Game.h"
#include "Map.h"

Unit Game::m_units[UNITS_MAX];
Player Game::m_player;
int16_t Game::m_draw_offset_x = 0;
int16_t Game::m_draw_offset_y = 0;

void Game::Init()
{
  //Clear player
  m_player.flags = 0;
  m_player.lives = 3;
  m_player.bomb_maximum = 10;
  m_player.bomb_put = 0;
  m_player.bomb_radius = 5;
}

void Game::StartLevel()
{
  Map::Init(17, 9);

  //Prepare player
  Player::LevelReset(&m_player);
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
  Map::Control(frame_number);
  Player::Control(&m_player, buttons, frame_number);
  
  for (uint8_t i = 0; i < UNITS_MAX; ++i)
  {
    if ((uint8_t)(m_units[i].flags & UNIT_FLAG_ALIVE) != 0)
      Unit::Move(m_units+i, frame_number+i);
  }
}

void Game::Draw(uint16_t frame_number)
{
  if (Map::m_width == 17)
    m_draw_offset_x = -4;
  else
  {
    m_draw_offset_x = (64-4)-(int16_t)m_player.x;
    if (Map::m_width*8+m_draw_offset_x < 128)
      m_draw_offset_x = 128-(int16_t)Map::m_width*8;
    if (m_draw_offset_x > 0)
      m_draw_offset_x = 0;
  }

  if (Map::m_height == 9)
    m_draw_offset_y= -4;
  else
  {
    m_draw_offset_y = (32-4)-(int16_t)m_player.y;
    if (Map::m_height*8+m_draw_offset_y < 128)
    m_draw_offset_y = 128-(int16_t)Map::m_height*8;
    if (m_draw_offset_y > 0)
      m_draw_offset_y = 0;
  }
    
  Map::Draw();

  Player::Draw(&m_player);
  for (uint8_t i = 0; i < UNITS_MAX; ++i)
  {
    if ((uint8_t)(m_units[i].flags & UNIT_FLAG_ACTIVE) != 0)
      Unit::Draw(m_units+i, frame_number+i);
  }
}
