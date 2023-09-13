#pragma once

#include <QuEngine/QuAnimSheet.h>
#include <memory>
#include <string>

class QuSprite;
class QuAnimSystem;

class QuAnimSheetComp
{
  friend QuAnimSystem;

public:
  QuAnimSheetComp(std::shared_ptr<QuAnimSheet> sheet,
                  std::shared_ptr<QuSprite> target);

  void Play(const std::string& anim);

private:
  void Update(float elapsed);

  float m_TimeAcc = 0;
  uint8_t m_CurrentFrame = 0;
  QuAnimSheet::QuSpriteAnim m_CurrentAnim;
  std::shared_ptr<QuAnimSheet> m_Sheet;
  std::shared_ptr<QuSprite> m_Target;
};