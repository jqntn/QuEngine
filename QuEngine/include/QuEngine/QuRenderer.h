#pragma once

#include <glm/fwd.hpp>

class QuTexture;
struct QuRect;
struct SDL_Renderer;

class QuRenderer
{
public:
  QuRenderer();
  virtual ~QuRenderer();

  static const QuRenderer* Current() { return s_pCurrentRenderer; }

  SDL_Renderer* GetRaw() const { return m_pRenderer; }

  void SetClearColor(glm::uvec4 color) const;

  void RenderCopy(const QuTexture& tex,
                  const QuRect& src,
                  const QuRect& dst) const;
  void Clear() const;
  void Present() const;

protected:
  SDL_Renderer* m_pRenderer = nullptr;

private:
  inline static const QuRenderer* s_pCurrentRenderer;
};