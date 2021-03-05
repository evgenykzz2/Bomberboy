#include <Arduboy2.h>
#include "defines.h"
#include "Menu.h"
#include "Game.h"

Arduboy2 arduboy;
//ArduboyTones sound(arduboy.audio.enabled);
Sprites sprites;

#define MODE_MENU 0
#define MODE_GAME 1

static uint8_t s_mode;

void setup()
{
  arduboy.begin();
  arduboy.setFrameRate(TARGET_FRAMERATE);
  
  s_mode = MODE_MENU;
  Menu::Init();

  //Game::Init();
  //Game::StartLevel();
}

void loop()
{
  if (!arduboy.nextFrame())
    return;

  if (s_mode == MODE_MENU)
  {
    if (Menu::Control(Arduboy2Core::buttonsState(), arduboy.frameCount))
    {
      s_mode = MODE_GAME;
      Game::Init();
      Game::StartLevel();
    } else
      Menu::Draw(arduboy);
  } else if (s_mode == MODE_GAME)
  {
    if (Game::Control(Arduboy2Core::buttonsState(), arduboy.frameCount))
    {
      s_mode = MODE_MENU;
      Menu::Init();
    } else
      Game::Draw(arduboy);
  }
  arduboy.display(true);
}
