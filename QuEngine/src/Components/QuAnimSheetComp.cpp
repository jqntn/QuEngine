#include <QuEngine/Components/QuAnimSheetComp.h>
#include <QuEngine/QuSprite.h>

QuAnimSheetComp::QuAnimSheetComp(std::shared_ptr<QuAnimSheet> sheet,
                                 std::shared_ptr<QuSprite> target)
{
  m_Sheet = std::move(sheet);
  m_Target = std::move(target);
}

void
QuAnimSheetComp::Play(const std::string& anim)
{
  m_TimeAcc = 0;
  m_CurrentFrame = 0;
  m_CurrentAnim = m_Sheet->GetAnim(anim);
}

void
QuAnimSheetComp::Update(float elapsed)
{
  m_TimeAcc += elapsed;
  while (m_TimeAcc >= m_CurrentAnim.FrameTime) {
    m_TimeAcc -= m_CurrentAnim.FrameTime;
    if (++m_CurrentFrame >= m_CurrentAnim.FrameCount)
      m_CurrentFrame = 0;

    m_Target->SetRect(
      QuRect(m_CurrentAnim.Start.x + m_CurrentAnim.Size.x * m_CurrentFrame,
             m_CurrentAnim.Start.y,
             m_CurrentAnim.Size.x,
             m_CurrentAnim.Size.y));
  }
}