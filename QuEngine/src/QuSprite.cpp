#include <QuEngine/QuRenderer.h>
#include <QuEngine/QuSprite.h>
#include <SDL2/SDL_render.h>
#include <glm/ext/matrix_float3x3.hpp>
#include <vector>

QuSprite::QuSprite(std::shared_ptr<QuTexture> texture)
  : QuSprite(std::move(texture), texture->GetRect())
{
}

QuSprite::QuSprite(std::shared_ptr<QuTexture> texture, QuRect rect)
{
  m_Texture = std::move(texture);
  m_Rect = rect;
  m_Size = glm::vec2(rect.w, rect.h);
}

void
QuSprite::Render(glm::mat4 matrix)
{
  auto o = glm::vec4(m_Origin.x * m_Size.x, m_Origin.y * m_Size.y, 0, 0);
  auto TL = matrix * (glm::vec4(0, 0, 0, 1) - o);
  auto TR = matrix * (glm::vec4(m_Size.x, 0, 0, 1) - o);
  auto BL = matrix * (glm::vec4(0, m_Size.y, 0, 1) - o);
  auto BR = matrix * (glm::vec4(m_Size.x, m_Size.y, 0, 1) - o);

  auto r = m_Rect;
  auto rt = m_Texture->GetRect();
  auto UV = glm::vec2(1.f / rt.w, 1.f / rt.h);

  SDL_Vertex v[4]{};
  v[0].color = { 255, 255, 255, 255 };
  v[1].color = { 255, 255, 255, 255 };
  v[2].color = { 255, 255, 255, 255 };
  v[3].color = { 255, 255, 255, 255 };
  v[0].position = { TL.x, TL.y };
  v[1].position = { TR.x, TR.y };
  v[2].position = { BL.x, BL.y };
  v[3].position = { BR.x, BR.y };
  v[0].tex_coord = { r.x * UV.x, r.y * UV.y };
  v[1].tex_coord = { (r.x + r.w) * UV.x, r.y * UV.y };
  v[2].tex_coord = { r.x * UV.x, (r.y + r.h) * UV.y };
  v[3].tex_coord = { (r.x + r.w) * UV.x, (r.y + r.h) * UV.y };

  int32_t i[6]{ 0, 1, 2, 2, 1, 3 };

  SDL_RenderGeometry(
    QuRenderer::Current()->GetRaw(), m_Texture->GetRaw(), v, 4, i, 6);
}