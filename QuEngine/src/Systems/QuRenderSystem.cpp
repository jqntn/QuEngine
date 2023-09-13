#include <QuEngine/Components/QuRenderableComp.h>
#include <QuEngine/QuCamera.h>
#include <QuEngine/QuSprite.h>
#include <QuEngine/QuTransform.h>
#include <QuEngine/Systems/QuRenderSystem.h>
#include <entt/entt.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float3x3.hpp>

QuRenderSystem::QuRenderSystem(entt::registry& registry)
  : m_Registry(registry)
{
}

void
QuRenderSystem::Run()
{
  auto camMatrix = glm::mat4(1);
  auto camView = m_Registry.view<QuTransform, QuCamera>();
  auto camFound = false;
  for (auto entity : camView) {
    if (camFound)
      throw std::runtime_error("Found Multiple Camera");
    camFound = true;
    camMatrix =
      glm::inverse(camView.get<QuTransform>(entity).GetTransformMatrix());
  }
  if (!camFound)
    throw std::runtime_error("Found No Camera");

  auto view = m_Registry.view<QuTransform, QuRenderableComp>();
  for (auto entity : view)
    view.get<QuRenderableComp>(entity).Renderable->Render(
      camMatrix * view.get<QuTransform>(entity).GetTransformMatrix());
}