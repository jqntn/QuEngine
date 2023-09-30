#pragma once

#include <QuEngine/QuRenderable.h>
#include <QuEngine/QuTexture.h>
#include <glm/ext/vector_float2.hpp>
#include <memory>

class QuTexture;

class QuSprite : public QuRenderable
{
public:
  QuSprite(std::shared_ptr<QuTexture> texture);
  QuSprite(std::shared_ptr<QuTexture> texture, QuRect rect);

  QuRect GetRect() const { return m_Rect; }
  glm::vec2 GetSize() const { return m_Size; }
  glm::vec2 GetOrigin() const { return m_Origin; }

  void SetRect(QuRect rect) { m_Rect = rect; }
  void SetSize(glm::vec2 size) { m_Size = size; }
  void SetOrigin(glm::vec2 origin) { m_Origin = origin; }

  void Render(glm::mat4 matrix) override;

private:
  std::shared_ptr<QuTexture> m_Texture;
  QuRect m_Rect{};
  glm::vec2 m_Size{};
  glm::vec2 m_Origin{};
};