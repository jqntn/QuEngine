#include <QuEngine/QuEvent.h>
#include <QuEngine/QuGUI.h>
#include <QuEngine/QuRenderer2D.h>
#include <QuEngine/QuWindow.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

QuGUI::QuGUI(const QuWindow& window, const QuRenderer& renderer)
{
  m_pContext = ImGui::CreateContext();

  ImGui::SetCurrentContext(m_pContext);
  ImGui::GetIO().IniFilename = NULL;

  ImGui_ImplSDL2_InitForSDLRenderer(window.GetRaw(), renderer.GetRaw());
  ImGui_ImplSDLRenderer2_Init(renderer.GetRaw());
}

QuGUI::~QuGUI()
{
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();

  ImGui::DestroyContext(m_pContext);
}

void
QuGUI::HandleEvent(const QuEvent& event)
{
  ImGui_ImplSDL2_ProcessEvent(event.GetRaw());
}

void
QuGUI::NewFrame()
{
  ImGui_ImplSDLRenderer2_NewFrame();
  ImGui_ImplSDL2_NewFrame();

  ImGui::NewFrame();
}

void
QuGUI::Render()
{
  ImGui::Render();

  ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}