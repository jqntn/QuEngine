#pragma once

#include <QuEngine/QuSprite.h>
#include <array>
#include <entt/entity/fwd.hpp>
#include <glm/ext/vector_uint2.hpp>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#define idx(x, y) x + 8 * y

class QuTexture;

struct Piece
{
  enum class Type
  {
    King,
    Queen,
    Bishop,
    Knight,
    Rook,
    Pawn,
  } Type = Type::Pawn;
  enum class Color
  {
    White,
    Black,
  } Color = Color::White;
  std::string Owner;
};

class Board
{
public:
  Board();

  void Render();

private:
  QuSprite MakeTile(std::shared_ptr<QuTexture> texture, glm::ivec2 position);
  entt::entity MakePiece(std::string name);
  void MakeBoard();
  void PopulateBoard();
  void ClearBoard();
  void ShowBoard();
  std::optional<entt::entity> GetHoveredPiece();

  const glm::ivec2 TILE_SIZE{ 64, 64 };
  const glm::ivec2 PIECE_SIZE{ 64, 64 };
  const glm::ivec2 BOARD_SIZE{ 8, 8 };

  std::unordered_map<std::string, QuSprite> m_Tiles;
  std::array<std::optional<Piece>, 8 * 8> m_Board;
  entt::entity m_Camera;
  entt::entity m_Selector;
};