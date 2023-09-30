#pragma once

#pragma warning(push)
#pragma warning(disable : 4334)
#pragma warning(disable : 4715)
#include <Chess/chess-engine/chess.hpp>
#pragma warning(pop)

#include <QuEngine/QuSprite.h>
#include <entt/entity/fwd.hpp>

#define idx(x, y) x + 8 * y
#define crd(i) glm::ivec2(i % 8, (i / 8) % 8)
#define cr(c)                                                                  \
  (c = (m_pClient->Room.P1.Color == chess::Color::WHITE)                       \
         ? glm::ivec2(c.x, std::abs(BOARD_SIZE.y - c.y - 1))                   \
         : c)

class QuTexture;
class Client;
struct Player;

struct Selector
{};
struct Indicator
{};
struct Piece
{};
inline std::string
operator|(chess::Color color, chess::PieceType type)
{
  return std::to_string((uint8_t)color) + std::to_string((uint8_t)type);
}

class Board
{
public:
  Board();
  ~Board();

  void Tick();
  void Render();
  void RenderPlayer(const Player& player);

private:
  QuSprite MakeTile(std::shared_ptr<QuTexture> texture, glm::ivec2 position);
  void MakeBoard();
  void ShowBoard();
  void ShowIndicators();
  void FindLegalMoves(std::optional<entt::entity> piece);
  void MakeMove(const chess::Move& move);
  std::optional<entt::entity> GetHoveredPiece();
  glm::ivec2 GetHoveredSquare();

  const glm::ivec2 TILE_SIZE{ 64, 64 };
  const glm::ivec2 PIECE_SIZE{ 64, 64 };
  const glm::ivec2 BOARD_SIZE{ 8, 8 };

  std::unordered_map<std::string, QuSprite> m_Tiles;
  chess::Board m_Board;
  entt::entity m_Camera;

  std::pair<std::optional<entt::entity>, std::vector<chess::Move>> m_LegalMoves;
  std::unordered_map<chess::Color, std::string> m_LastMove;
  std::pair<chess::GameResultReason, chess::GameResult> m_GameState = {
    chess::GameResultReason::NONE,
    chess::GameResult::NONE
  };

  Client* m_pClient = nullptr;
  bool m_IsMakeBoardDirty = false;
  chess::Move m_ReceivedMove;
};