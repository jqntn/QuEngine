#include <QuEngine/Managers/QuInputManager.h>
#include <QuEngine/QuEngine.h>
#include <QuEngine/QuEvent.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <fstream>
#include <iostream>

void
HelloQuEngine()
{
  puts("QuEngine");
}

QuEngine::QuEngine()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    throw std::runtime_error("SDL_Init: " + std::string(SDL_GetError()));

  if (Mix_OpenAudio(48000, AUDIO_F32SYS, 2, 4096) < 0)
    throw std::runtime_error("Mix_OpenAudio: " + std::string(Mix_GetError()));

  Mix_AllocateChannels(32);
}

QuEngine::~QuEngine()
{
  Mix_CloseAudio();

  SDL_Quit();
}

void
QuEngine::StartCounters()
{
  m_Then = SDL_GetPerformanceCounter();
}

void
QuEngine::UpdateCounters()
{
  m_Now = SDL_GetPerformanceCounter();
  m_DeltaTime = (float)(m_Now - m_Then) / SDL_GetPerformanceFrequency();
  m_Then = m_Now;
}

bool
QuEngine::PollEvent(const QuEvent& event)
{
  auto pending = SDL_PollEvent(event.GetRaw());

  if (pending > 0)
    QuInputManager::Instance().HandleEvent(event);

  return pending;
}

void
LogError(const std::string& what)
{
  std::cout << "ERROR: " << what << '\n';
}

void
SaveLogs()
{
  auto dir = GetExePath().parent_path().string();
  static auto of = std::ofstream(dir + "/log.txt");
  std::cout.rdbuf(of.rdbuf());
}

std::filesystem::path
GetExePath()
{
  auto path = SDL_GetBasePath();

  if (path == nullptr)
    throw std::runtime_error("SDL_GetBasePath: " + std::string(SDL_GetError()));

  return std::filesystem::path(path);
}