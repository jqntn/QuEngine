#pragma once

#include <string>

struct SDL_Surface;

class QuSurface
{
public:
  QuSurface(uint32_t w, uint32_t h);
  ~QuSurface();

  SDL_Surface* GetRaw() const { return m_pSurface; }

  static QuSurface LoadFromFile(const std::string& path);
  static QuSurface LoadFromMemory(const std::byte* bytes, size_t size);

private:
  QuSurface(SDL_Surface* pSurface);

  SDL_Surface* m_pSurface = nullptr;
};
