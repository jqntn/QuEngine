#include <QuEngine/QuTransform.h>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/mat3x3.hpp>

QuTransform::QuTransform(const QuTransform& other)
{
  m_Position = other.m_Position;
  m_Scale = other.m_Scale;
  m_Rotation = other.m_Rotation;

  SetParent(other.m_Parent);
}

QuTransform&
QuTransform::operator=(const QuTransform& other)
{
  m_Position = other.m_Position;
  m_Scale = other.m_Scale;
  m_Rotation = other.m_Rotation;

  SetParent(other.m_Parent);

  return *this;
}

glm::vec2
QuTransform::GetWorldPosition() const
{
  return m_Parent ? m_Parent->TransformPoint(m_Position) : m_Position;
}

glm::vec2
QuTransform::GetWorldScale() const
{
  return m_Parent ? m_Parent->GetWorldScale() * m_Scale : m_Scale;
}

float
QuTransform::GetWorldRotation() const
{
  return m_Parent ? m_Parent->GetWorldRotation() + m_Rotation : m_Rotation;
}

glm::mat4
QuTransform::GetTransformMatrix() const
{
  auto t = glm::translate(glm::vec3(m_Position.x, m_Position.y, 0));
  auto s = glm::scale(glm::vec3(m_Scale.x, m_Scale.y, 1));
  auto r = glm::rotate(glm::radians(m_Rotation), glm::vec3(0, 0, 1));

  auto m = t * r * s;

  if (m_Parent != nullptr)
    m *= m_Parent->GetTransformMatrix();

  return m;
}

void
QuTransform::SetParent(QuTransform* parent)
{
  if (m_Parent == parent)
    return;

  if (m_Parent != nullptr) {
    auto it = std::find(m_Children.begin(), m_Children.end(), this);
    if (it != m_Children.end())
      m_Children.erase(it);
  }

  m_Parent = parent;

  if (m_Parent != nullptr)
    m_Children.push_back(this);
}

glm::vec2
QuTransform::TransformPoint(glm::vec2 position) const
{
  position *= GetWorldScale();
  position = glm::rotate(position, GetWorldRotation());
  position += m_Parent ? m_Parent->TransformPoint(m_Position) : m_Position;

  return position;
}

glm::vec2
QuTransform::InverseTransformPoint(glm::vec2 position) const
{
  position -= m_Parent ? m_Parent->TransformPoint(m_Position) : m_Position;
  position = glm::rotate(position, -GetWorldRotation());
  position /= GetWorldScale();

  return position;
}