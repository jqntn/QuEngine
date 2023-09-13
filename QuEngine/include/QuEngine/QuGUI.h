#pragma once

class QuWindow;
class QuRenderer;
class QuEvent;
struct ImGuiContext;

class QuGUI
{
public:
  QuGUI(const QuWindow& window, const QuRenderer& renderer);
  ~QuGUI();

  ImGuiContext* GetRaw() const { return m_pContext; }

  void HandleEvent(const QuEvent& event);

  void NewFrame();
  void Render();

private:
  ImGuiContext* m_pContext = nullptr;
};