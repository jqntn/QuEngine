#pragma once

#include <glm/fwd.hpp>

class QuRenderer;

class QuRenderable
{
public:
  virtual ~QuRenderable() = default;
  virtual void Render(glm::mat4 matrix) = 0;
};