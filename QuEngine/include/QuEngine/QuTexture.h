#pragma once

#include <string>

class QuSurface;
struct SDL_Texture;
struct SDL_Rect;

struct QuRect
{
  int32_t x, y, w, h = 0;
};

class QuTexture
{
public:
  ~QuTexture();

  QuTexture(QuTexture&&) noexcept;
  QuTexture(const QuTexture&) = default;
  QuTexture& operator=(QuTexture&&) noexcept;
  QuTexture& operator=(const QuTexture&) = default;

  SDL_Texture* GetRaw() const { return m_pTexture; }
  QuRect GetRect() const;

  static QuTexture LoadFromFile(const std::string& path);
  static QuTexture LoadFromMemory(const std::byte* bytes, size_t size);
  static QuTexture LoadFromSurface(const QuSurface& surface);

private:
  QuTexture(SDL_Texture* pTexture);

  SDL_Texture* m_pTexture = nullptr;
};