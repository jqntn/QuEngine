#include <QuEngine/Managers/QuInputManager.h>
#include <QuEngine/QuEvent.h>
#include <QuEngine/QuTransform.h>
#include <SDL2/SDL_events.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_int2.hpp>
#include <glm/ext/vector_int4.hpp>

QuInputManager::~QuInputManager()
{
  s_Instance = nullptr;
}

bool
QuInputManager::IsActive(const std::string& action) const
{
  auto it = m_ActionToData.find(action);
  return it != m_ActionToData.end() ? it->second.IsActive : false;
}

void
QuInputManager::HandleEvent(const QuEvent& event)
{
  auto e = event.GetRaw();
  switch (e->type) {
    case SDL_MOUSEBUTTONDOWN: {
      auto it = m_MouseToAction.find(e->button.button);
      if (it != m_MouseToAction.end())
        Trigger(it->second);
    } break;
    case SDL_MOUSEBUTTONUP: {
      auto it = m_MouseToAction.find(e->button.button);
      if (it != m_MouseToAction.end())
        Release(it->second);
    } break;
    case SDL_KEYDOWN: {
      auto it = m_KeyToAction.find(e->key.keysym.sym);
      if (it != m_KeyToAction.end())
        Trigger(it->second);
    } break;
    case SDL_KEYUP: {
      auto it = m_KeyToAction.find(e->key.keysym.sym);
      if (it != m_KeyToAction.end())
        Release(it->second);
    } break;
    case SDL_CONTROLLERBUTTONDOWN: {
      auto it =
        m_GamepadToAction.find((SDL_GameControllerButton)e->cbutton.button);
      if (it != m_GamepadToAction.end())
        Trigger(it->second);
    } break;
    case SDL_CONTROLLERBUTTONUP: {
      auto it =
        m_GamepadToAction.find((SDL_GameControllerButton)e->cbutton.button);
      if (it != m_GamepadToAction.end())
        Release(it->second);
    } break;
  }
}

void
QuInputManager::OnAction(const std::string& action,
                         const std::function<void(bool)>& func)
{
  m_ActionToData[action].Func = func;
}

void
QuInputManager::BindKeyPress(SDL_KeyCode key, const std::string& action)
{
  m_KeyToAction[key] = action;
}

void
QuInputManager::BindMousePress(uint8_t button, const std::string& action)
{
  m_MouseToAction[button] = action;
}

void
QuInputManager::BindGamepadPress(SDL_GameControllerButton button,
                                 const std::string& action)
{
  m_GamepadToAction[button] = action;
}

glm::ivec2
QuInputManager::GetMousePosition()
{
  auto mousePosition = glm::ivec2();
  SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
  return mousePosition;
}

glm::ivec2
QuInputManager::GetMouseWorldPosition(const QuTransform& cameraTransform)
{
  auto mousePosition = QuInputManager::GetMousePosition();
  auto mouseVector = glm::ivec4(mousePosition.x, mousePosition.y, 0, 1);
  mouseVector = cameraTransform.GetTransformMatrix() * mouseVector;
  return glm::ivec2(mouseVector);
}

QuInputManager&
QuInputManager::Instance()
{
  if (s_Instance == nullptr)
    s_Instance = new QuInputManager();
  return *s_Instance;
}

void
QuInputManager::Trigger(const std::string& action)
{
  auto& actionData = m_ActionToData[action];
  actionData.IsActive = true;
  if (actionData.Func != nullptr)
    actionData.Func(true);
}

void
QuInputManager::Release(const std::string& action)
{
  auto& actionData = m_ActionToData[action];
  actionData.IsActive = false;
  if (actionData.Func != nullptr)
    actionData.Func(false);
}