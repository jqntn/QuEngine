#include <QuEngine/QuRenderer.h>
#include <QuEngine/QuSurface.h>
#include <QuEngine/QuTexture.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdexcept>

QuTexture::QuTexture(SDL_Texture* pTexture)
{
  m_pTexture = pTexture;
}

QuTexture::~QuTexture()
{
  SDL_DestroyTexture(m_pTexture);
}

QuTexture::QuTexture(QuTexture&& other) noexcept
{
  std::swap(m_pTexture, other.m_pTexture);
}

QuTexture&
QuTexture::operator=(QuTexture&& other) noexcept
{
  std::swap(m_pTexture, other.m_pTexture);
  return *this;
}

QuRect
QuTexture::GetRect() const
{
  auto rect = QuRect();
  SDL_QueryTexture(m_pTexture, nullptr, nullptr, &rect.w, &rect.h);
  return rect;
}

QuTexture
QuTexture::LoadFromFile(const std::string& path)
{
  return LoadFromSurface(QuSurface::LoadFromFile(path));
}

QuTexture
QuTexture::LoadFromMemory(const std::byte* bytes, size_t size)
{
  return LoadFromSurface(QuSurface::LoadFromMemory(bytes, size));
}

QuTexture
QuTexture::LoadFromSurface(const QuSurface& surface)
{
  auto pTexture = SDL_CreateTextureFromSurface(QuRenderer::Current()->GetRaw(),
                                               surface.GetRaw());

  if (pTexture == nullptr)
    throw std::runtime_error("SDL_CreateTextureFromSurface: " +
                             std::string(SDL_GetError()));

  return QuTexture(pTexture);
}