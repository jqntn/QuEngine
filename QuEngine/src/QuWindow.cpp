#include <QuEngine/QuWindow.h>
#include <SDL2/SDL_video.h>
#include <stdexcept>

QuWindow::QuWindow(const std::string& title, glm::uvec2 size, uint8_t flags)
{
  m_pWindow = SDL_CreateWindow(title.c_str(),
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               size.x,
                               size.y,
                               SDL_WINDOW_SHOWN | flags);

  if (m_pWindow == nullptr)
    throw std::runtime_error("SDL_CreateWindow: " +
                             std::string(SDL_GetError()));
}

QuWindow::~QuWindow()
{
  SDL_DestroyWindow(m_pWindow);
}