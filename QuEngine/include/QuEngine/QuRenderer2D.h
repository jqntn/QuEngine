#pragma once

#include <QuEngine/QuRenderer.h>

class QuWindow;

class QuRenderer2D : public QuRenderer
{
public:
  QuRenderer2D(const QuWindow& window);
};