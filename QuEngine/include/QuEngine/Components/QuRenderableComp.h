#pragma once

#include <memory>

class QuRenderable;

class QuRenderableComp
{
public:
  std::shared_ptr<QuRenderable> Renderable;
};