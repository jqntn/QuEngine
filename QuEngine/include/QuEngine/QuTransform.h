#pragma once

#include <glm/ext/vector_float2.hpp>
#include <glm/fwd.hpp>
#include <vector>

class QuTransform
{
public:
  QuTransform() = default;

  QuTransform(const QuTransform&);
  QuTransform(QuTransform&&) = delete;
  QuTransform& operator=(const QuTransform&);
  QuTransform& operator=(QuTransform&&) = delete;

  QuTransform* GetParent() const { return m_Parent; }
  const std::vector<QuTransform*>& GetChildren() const { return m_Children; }
  glm::vec2 GetLocalPosition() const { return m_Position; }
  glm::vec2 GetLocalScale() const { return m_Scale; }
  float GetLocalRotation() const { return m_Rotation; }
  glm::vec2 GetWorldPosition() const;
  glm::vec2 GetWorldScale() const;
  float GetWorldRotation() const;
  glm::mat4 GetTransformMatrix() const;

  void SetParent(QuTransform* parent);
  void SetLocalPosition(glm::vec2 position) { m_Position = position; }
  void SetLocalScale(glm::vec2 scale) { m_Scale = scale; }
  void SetLocalRotation(float rotation) { m_Rotation = rotation; }

  glm::vec2 TransformPoint(glm::vec2 position) const;
  glm::vec2 InverseTransformPoint(glm::vec2 position) const;

private:
  QuTransform* m_Parent = nullptr;

  std::vector<QuTransform*> m_Children;
  glm::vec2 m_Position{};
  glm::vec2 m_Scale{ 1, 1 };
  float m_Rotation = 0;
};