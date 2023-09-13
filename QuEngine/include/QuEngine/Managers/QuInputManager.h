#pragma once

#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_keycode.h>
#include <functional>
#include <glm/fwd.hpp>
#include <string>

class QuEvent;

class QuInputManager
{
public:
  ~QuInputManager();

  QuInputManager(const QuInputManager&) = delete;
  QuInputManager(QuInputManager&&) = delete;

  QuInputManager& operator=(const QuInputManager&) = delete;
  QuInputManager& operator=(QuInputManager&&) = delete;

  static QuInputManager& Instance();

  bool IsActive(const std::string& action) const;

  void HandleEvent(const QuEvent& event);
  void OnAction(const std::string& action,
                const std::function<void(bool)>& func);

  void BindKeyPress(SDL_KeyCode key, const std::string& action);
  void BindMousePress(uint8_t button, const std::string& action);
  void BindGamepadPress(SDL_GameControllerButton button,
                        const std::string& action);

  static glm::ivec2 GetMousePosition();

private:
  struct ActionData
  {
    bool IsActive = false;
    std::function<void(bool)> Func;
  };

  QuInputManager() = default;

  void Trigger(const std::string& action);
  void Release(const std::string& action);

  inline static QuInputManager* s_Instance;

  std::unordered_map<uint8_t, std::string> m_MouseToAction;
  std::unordered_map<SDL_Keycode, std::string> m_KeyToAction;
  std::unordered_map<SDL_GameControllerButton, std::string> m_GamepadToAction;
  std::unordered_map<std::string, ActionData> m_ActionToData;
};