#include <QuEngine/QuSurface.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <stdexcept>

QuSurface::QuSurface(uint32_t w, uint32_t h)
{
  m_pSurface =
    SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA32);
}

QuSurface::QuSurface(SDL_Surface* pSurface)
{
  m_pSurface = pSurface;
}

QuSurface::~QuSurface()
{
  SDL_FreeSurface(m_pSurface);
}

QuSurface
QuSurface::LoadFromFile(const std::string& path)
{
  auto pSurface = IMG_Load(path.c_str());

  if (pSurface == nullptr)
    throw std::runtime_error("IMG_Load: " + std::string(IMG_GetError()));

  return QuSurface(pSurface);
}

QuSurface
QuSurface::LoadFromMemory(const std::byte* bytes, size_t size)
{
  auto pRWOp = SDL_RWFromConstMem(bytes, (int32_t)size);

  if (pRWOp == nullptr)
    throw std::runtime_error("SDL_RWFromConstMem: " +
                             std::string(SDL_GetError()));

  auto pSurface = IMG_Load_RW(pRWOp, 1);

  if (pSurface == nullptr)
    throw std::runtime_error("IMG_Load_RW: " + std::string(IMG_GetError()));

  return QuSurface(pSurface);
}