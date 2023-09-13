#include <QuEngine/Components/QuAnimSheetComp.h>
#include <QuEngine/QuAnimSheet.h>
#include <QuEngine/Systems/QuAnimSystem.h>
#include <entt/entt.hpp>

QuAnimSystem::QuAnimSystem(entt::registry& registry)
  : m_Registry(registry)
{
}

void
QuAnimSystem::Run(float elapsed)
{
  auto view = m_Registry.view<QuAnimSheetComp>();
  for (auto entity : view)
    view.get<QuAnimSheetComp>(entity).Update(elapsed);
}