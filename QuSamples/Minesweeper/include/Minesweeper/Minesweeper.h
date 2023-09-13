#pragma once

#include <entt/entity/registry.hpp>

class Minesweeper
{
public:
  Minesweeper();

  inline static entt::registry s_Registry;
};