#include <QuEngine/QuEvent.h>
#include <SDL2/SDL_events.h>

QuEvent::QuEvent()
{
  m_pEvent = new SDL_Event();
}

QuEvent::~QuEvent()
{
  delete m_pEvent;
}

bool
QuEvent::IsQuit()
{
  return m_pEvent->type == SDL_QUIT;
}