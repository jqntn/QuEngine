#include <QuEngine/QuRenderer2D.h>
#include <QuEngine/QuWindow.h>
#include <SDL2/SDL_render.h>
#include <stdexcept>

QuRenderer2D::QuRenderer2D(const QuWindow& window)
{
  m_pRenderer = SDL_CreateRenderer(
    window.GetRaw(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (m_pRenderer == nullptr)
    throw std::runtime_error("SDL_CreateRenderer: " +
                             std::string(SDL_GetError()));
}