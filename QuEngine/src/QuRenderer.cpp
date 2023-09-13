#include <QuEngine/QuRenderer.h>
#include <QuEngine/QuTexture.h>
#include <SDL2/SDL_render.h>
#include <glm/ext/vector_uint4.hpp>

QuRenderer::QuRenderer()
{
  m_pRenderer = nullptr;

  s_pCurrentRenderer = this;
}

QuRenderer::~QuRenderer()
{
  SDL_DestroyRenderer(m_pRenderer);

  s_pCurrentRenderer = nullptr;
}

void
QuRenderer::SetClearColor(glm::uvec4 color) const
{
  SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, color.a);
}

void
QuRenderer::RenderCopy(const QuTexture& tex,
                       const QuRect& src,
                       const QuRect& dst) const
{
  auto srcRct = SDL_Rect(src.x, src.y, src.w, src.h);
  auto dstRct = SDL_Rect(dst.x, dst.y, dst.w, dst.h);
  SDL_RenderCopy(m_pRenderer, tex.GetRaw(), &srcRct, &dstRct);
}

void
QuRenderer::Clear() const
{
  SDL_RenderClear(m_pRenderer);
}

void
QuRenderer::Present() const
{
  SDL_RenderPresent(m_pRenderer);
}