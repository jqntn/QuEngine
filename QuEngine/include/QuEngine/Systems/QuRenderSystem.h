#pragma once

#include <entt/entity/fwd.hpp>

class QuRenderSystem
{
public:
  QuRenderSystem(entt::registry& registry);

  void Run();

private:
  entt::registry& m_Registry;
};