#pragma once

union SDL_Event;

class QuEvent
{
public:
  QuEvent();
  ~QuEvent();

  SDL_Event* GetRaw() const { return m_pEvent; }

  bool IsQuit();

private:
  SDL_Event* m_pEvent = nullptr;
};