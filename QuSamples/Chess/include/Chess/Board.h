#pragma once

#include <QuEngine/QuSprite.h>
#include <array>
#include <bitset>
#include <entt/entity/fwd.hpp>
#include <glm/ext/vector_uint2.hpp>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#define idx(x, y) x + 8 * y
#define coord(i) glm::ivec2(i % 8, (i / 8) % 8)
#define checked(c)                                                             \
  (c = (m_Color == Color::White)                                               \
         ? glm::ivec2(c.x, std::abs(BOARD_SIZE.y - c.y - 1))                   \
         : c)

class QuTexture;

struct Selector
{};
struct Indicator
{};
struct Piece
{};
enum class Color : uint8_t
{
  White = 0,
  Black = 1 << 0,
};
enum class Type : uint8_t
{
  King = 0,
  Queen = 1 << 1,
  Bishop = 1 << 2,
  Knight = 1 << 3,
  Rook = 1 << 4,
  Pawn = 1 << 5,
};
inline uint8_t
operator|(Color color, Type type)
{
  return (uint8_t)color | (uint8_t)type;
}
inline uint8_t
operator&(uint8_t piece, Color color)
{
  return piece & (uint8_t)color;
}
inline uint8_t
operator&(uint8_t piece, Type type)
{
  return piece & (uint8_t)type;
}

class Board
{
public:
  Board();

  void Render();

private:
  QuSprite MakeTile(std::shared_ptr<QuTexture> texture, glm::ivec2 position);
  void MakeBoard();
  void PopulateBoard();
  void ClearBoard();
  void ShowBoard();
  void FindLegalMoves(std::optional<entt::entity> piece);
  void ShowIndicators();
  std::optional<entt::entity> GetHoveredPiece();

  const glm::ivec2 TILE_SIZE{ 64, 64 };
  const glm::ivec2 PIECE_SIZE{ 64, 64 };
  const glm::ivec2 BOARD_SIZE{ 8, 8 };

  std::unordered_map<std::string, QuSprite> m_Tiles;
  std::array<std::optional<uint8_t>, 8 * 8> m_Board;
  entt::entity m_Camera;

  bool m_canPlay = true;
  Color m_Color = Color::White;
  std::pair<std::optional<entt::entity>, std::vector<uint8_t>> m_LegalMoves;
};