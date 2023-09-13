#include <QuEngine/QuAnimSheet.h>

QuAnimSheet::QuSpriteAnim
QuAnimSheet::GetAnim(const std::string& anim) const
{
  return m_Animations.at(anim);
}

void
QuAnimSheet::AddAnim(const std::string& anim,
                     glm::uvec2 size,
                     glm::uvec2 start,
                     uint8_t frameCount,
                     float frameTime)
{
  m_Animations.emplace(anim, QuSpriteAnim(size, start, frameCount, frameTime));
}