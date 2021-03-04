#include <Arduboy2.h>
#include "defines.h"
#include "Game.h"

Arduboy2 arduboy;
//ArduboyTones sound(arduboy.audio.enabled);
Sprites sprites;

void setup()
{
  arduboy.begin();
  arduboy.setFrameRate(TARGET_FRAMERATE);

  Game::Init();
  Game::StartLevel();
}

void loop()
{
  if (!arduboy.nextFrame())
    return;
  Game::Control(Arduboy2Core::buttonsState(), arduboy.frameCount);

  Game::Draw(arduboy.frameCount);

  arduboy.display(true);
}
