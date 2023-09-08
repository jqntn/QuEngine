#include <QuEngine/QuEngine.h>
#include <SDL2/SDL.h>
#include <stdio.h>

void
HelloQuEngine()
{
  puts("QuEngine");
}

QuEngine::QuEngine()
{
  SDL_Init(0);
}

QuEngine::~QuEngine()
{
  SDL_Quit();
}