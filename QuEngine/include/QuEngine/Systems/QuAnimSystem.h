#pragma once

#include <entt/entity/fwd.hpp>

class QuAnimSystem
{
public:
  QuAnimSystem(entt::registry& registry);

  void Run(float elapsed);

private:
  entt::registry& m_Registry;
};