#pragma once

#include <QuEngine/QuSprite.h>
#include <entt/entity/fwd.hpp>
#include <optional>
#include <unordered_map>

class QuTexture;

struct Cell
{
  enum class Type
  {
    Empty,
    Number,
    Bomb,
  } Type = Type::Empty;
  enum class State
  {
    Veiled,
    Unveiled,
    Flagged,
    Reserved,
  } State = State::Veiled;
  size_t BombCount = 0;
};

class Board
{
public:
  Board();

  void Render();

private:
  QuSprite MakeTile(std::shared_ptr<QuTexture> texture, glm::ivec2 position);
  void MakeGrid();
  void PopulateGrid();
  std::optional<entt::entity> GetHoveredCell();
  std::vector<entt::entity> GetAdjacentCells(entt::entity centerCell);
  void UnveilCell(entt::entity cell);
  void FlagCell(entt::entity cell);

  const glm::ivec2 TILE_SIZE{ 16, 16 };
  const glm::ivec2 GRID_SIZE{ 16, 16 };
  const glm::ivec2 CELL_SIZE{ 32, 32 };

  std::unordered_map<std::string, QuSprite> m_Tiles;
  entt::entity m_Camera;
  bool m_IsGridPopulated = false;
  bool m_IsMuted = false;
  float m_Difficulty = .1f;
  size_t m_FlagCount = 0;
};