#pragma once

#include <glm/ext/vector_uint2.hpp>
#include <string>

struct SDL_Window;

class QuWindow
{
public:
  QuWindow(const std::string& title,
           glm::uvec2 size = { 720, 480 },
           uint8_t flags = 0);
  ~QuWindow();

  SDL_Window* GetRaw() const { return m_pWindow; }

private:
  SDL_Window* m_pWindow = nullptr;
};