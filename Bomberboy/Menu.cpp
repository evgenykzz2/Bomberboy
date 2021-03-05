#include "Menu.h"
#include "assets.h"
#include "Game.h"
#include "Map.h"

int16_t Menu::m_bomb_speed;
int16_t Menu::m_bomb_pos;
uint8_t Menu::m_logo_pos;
  
void Menu::Init()
{
  m_bomb_speed = 0;
  m_bomb_pos = -16*32;
  m_logo_pos = 128;

  //Use bomb to animate particle
  for (uint8_t i = 0; i < BOMBS_MAX; ++i)
    Map::m_bombs[i].activation_time = random(80) + 11;
}

bool Menu::Control(uint8_t buttons, uint16_t frame_number)
{
  //Animate bomb
  m_bomb_speed += 4;
  m_bomb_pos += m_bomb_speed;
  if ( (m_bomb_pos >> 5) >= 16 && m_bomb_speed > 0)
  {
    m_bomb_pos = 16 << 5;
    if (m_bomb_speed > (3 << 5))
      m_bomb_speed = (3 << 5);
    m_bomb_speed = -m_bomb_speed;
  }

  //Move logo
  m_logo_pos = (m_logo_pos*110 + 12*18) >> 7;

  for (uint8_t i = 0; i < BOMBS_MAX; ++i)
  {
    if (Map::m_bombs[i].activation_time == 0)
    {
      Map::m_bombs[i].cell_x = 32+20;
      Map::m_bombs[i].cell_y = (m_bomb_pos >> 5) + 0;
      Map::m_bombs[i].radius = random(PARTICLE_COUNT);
      Map::m_bombs[i].radius_counter = 0;
      Map::m_bombs[i].activation_time = 255;
    } else if (Map::m_bombs[i].activation_time != 255)
      Map::m_bombs[i].activation_time--;
    else
    {
      Map::m_bombs[i].radius_counter++;
      if (Map::m_bombs[i].radius_counter >= PARTICLE_FRAMES)
      {
        Map::m_bombs[i].cell_x = 32+20;
        Map::m_bombs[i].cell_y = (m_bomb_pos >> 5) + 0;
        Map::m_bombs[i].radius = random(PARTICLE_COUNT);
        Map::m_bombs[i].radius_counter = 0;
      }
    }
  }

  if ( (uint8_t)(buttons & A_BUTTON) != 0 )
    return true;

  return false;
}

void Menu::Draw(Arduboy2& arduboy)
{
  Arduboy2::drawBitmap(m_logo_pos, 13, s_logo, 112, 32);
  Sprites::drawPlusMask(32, m_bomb_pos >> 5, s_bomb, 0);
  arduboy.setCursor(28 - m_logo_pos, 50);
  arduboy.print(F("Press A to start"));

  for (uint8_t i = 0; i < BOMBS_MAX; ++i)
  {
    if (Map::m_bombs[i].activation_time != 255)
      continue;
    uint16_t index = (uint16_t)Map::m_bombs[i].radius*PARTICLE_FRAMES*2 + (uint16_t)Map::m_bombs[i].radius_counter*2;
    int8_t dx = (int8_t)pgm_read_byte(s_particle + index );
    int8_t dy = (int8_t)pgm_read_byte(s_particle + index + 1 );
    Arduboy2::drawPixel((int16_t)Map::m_bombs[i].cell_x + dx, (int16_t)Map::m_bombs[i].cell_y + dy);
  }
}
