#pragma once

#include <glm/ext/vector_uint2.hpp>
#include <optional>
#include <string>
#include <unordered_map>

class QuAnimSheet
{
public:
  struct QuSpriteAnim
  {
    glm::uvec2 Size;
    glm::uvec2 Start;
    uint8_t FrameCount;
    float FrameTime;
  };

  QuSpriteAnim GetAnim(const std::string& anim) const;

  void AddAnim(const std::string& anim,
               glm::uvec2 size,
               glm::uvec2 start,
               uint8_t frameCount,
               float frameTime);

private:
  std::unordered_map<std::string, QuSpriteAnim> m_Animations;
};