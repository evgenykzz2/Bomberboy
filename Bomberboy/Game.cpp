#include "Game.h"
#include "Map.h"
#include "assets.h"

Unit Game::m_units[UNITS_MAX];
Player Game::m_player;
int16_t Game::m_draw_offset_x = 0;
int16_t Game::m_draw_offset_y = 0;
uint8_t Game::m_level;
uint8_t Game::m_splash_level;
uint8_t Game::m_pause_toggle;
uint8_t Game::m_pause_mode;
uint8_t Game::m_ghost_left;

void Game::Init()
{
  //Clear player
  m_player.upgrade = 0; //PLAYER_UPGRADE_SPEED_3 | PLAYER_UPGRADE_GO_THROUGH_BOMBS;
  m_player.flags = 0;
  m_player.lives = 3;
  m_player.bomb_maximum = 1;
  m_player.bomb_put = 0;
  m_player.bomb_radius = 1;
  m_level = 1;
  m_pause_toggle = 0;
  m_ghost_left = 0;
}

void Game::StartLevel()
{
  m_splash_level = LEVEL_TITLE_DURATION_FRAMES;
  
  //Map::Init(31, 15);  //Maximum
  Map::Init(17, 9);  //Minimum

  //Prepare player
  Player::LevelReset(&m_player);
  //Get some free space for player
  *(Map::m_cell + STARTUP_CELL_Y * MAP_WIDTH_MAX + STARTUP_CELL_X) = CELL_EMPTY;
  *(Map::m_cell + STARTUP_CELL_Y * MAP_WIDTH_MAX + STARTUP_CELL_X+1) = CELL_EMPTY;
  *(Map::m_cell + (STARTUP_CELL_Y+1) * MAP_WIDTH_MAX + STARTUP_CELL_X) = CELL_EMPTY;
  
  //Clear enemies
  for (uint8_t i = 0; i < UNITS_MAX; ++i)
    m_units[i].flags = 0;

  //Put emenies
  uint8_t enemy_index = 0;
  m_ghost_left = 0;
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
    m_ghost_left ++;
    if (enemy_index >= 2)
      break;
    if (enemy_index >= UNITS_MAX)
      break;
  }
}

bool Game::Control(uint8_t buttons, uint16_t frame_number)
{
  if (m_splash_level != 0)
    m_splash_level --;

  if (m_splash_level != 0)
    return false;

  //Enter to pause screen
  if ( (uint8_t)(buttons & B_BUTTON) != 0 )
  {
    m_pause_toggle ++;
    if (m_pause_toggle >= PAUSE_PRESS_DURATION)
    {
      m_pause_toggle = 0;
      m_pause_mode ^= 1;
      return;
    }
  } else
    m_pause_toggle = 0;

  if (m_pause_mode)
    return false;

  Map::Control(frame_number);
  Player::Control(&m_player, buttons, frame_number);
  
  for (uint8_t i = 0; i < UNITS_MAX; ++i)
  {
    if ((uint8_t)(m_units[i].flags & UNIT_FLAG_ACTIVE) != 0)
      Unit::Control(m_units+i, frame_number+i);
  }

  return (m_player.lives == 255 && m_splash_level == 0 && m_player.flags == 0);
}

void Game::Draw(Arduboy2& arduboy)
{
  if (m_splash_level != 0)
  {
    if (m_player.lives == 255)
    {
      arduboy.setCursor(38, 32);
      arduboy.print(F("GAME OVER"));
    } else
    {
      arduboy.setCursor(48, 32);
      arduboy.print(F("Level "));
      arduboy.print(m_level);
      arduboy.setCursor(48, 56);
      arduboy.print(F("Lives "));
      arduboy.print(m_player.lives);
    }
  } else if (m_pause_mode)
  {
    int16_t bomb_x = 82;
    int16_t bomb_y = 24;
    Sprites::drawPlusMask(bomb_x, bomb_y, s_bomb, 0);
    for (uint8_t i = 0; i < PARTICLE_COUNT; ++i)
    {
      uint16_t index = (uint16_t)i*PARTICLE_FRAMES*2 + ((arduboy.frameCount + i*3) & (PARTICLE_FRAMES-1))*2;
      int8_t dx = (int8_t)pgm_read_byte(s_particle + index );
      int8_t dy = (int8_t)pgm_read_byte(s_particle + index + 1 );
      Arduboy2::drawPixel(bomb_x + dx + 20, bomb_y + dy + 0);
    }

    arduboy.setCursor(8,  8); arduboy.print(F("Level  ")); arduboy.print(m_level);
    arduboy.setCursor(8, 16); arduboy.print(F("Ghost  ")); arduboy.print(m_ghost_left);
    arduboy.setCursor(8, 24); arduboy.print(F("Lives  ")); arduboy.print(m_player.lives);
    arduboy.setCursor(8, 32); arduboy.print(F("Bombs  ")); arduboy.print(m_player.bomb_maximum);
    arduboy.setCursor(8, 40); arduboy.print(F("Radius ")); arduboy.print(m_player.bomb_radius);
    
  } else
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
      if (Map::m_height*8+m_draw_offset_y < 64)
      m_draw_offset_y = 64-(int16_t)Map::m_height*8;
      if (m_draw_offset_y > 0)
        m_draw_offset_y = 0;
    }
      
    Map::Draw();
  
    Player::Draw(&m_player, arduboy.frameCount);
    for (uint8_t i = 0; i < UNITS_MAX; ++i)
    {
      if ((uint8_t)(m_units[i].flags & UNIT_FLAG_ACTIVE) != 0)
        Unit::Draw(m_units+i, arduboy.frameCount+i);
    }
  }
}
