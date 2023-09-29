#include <Chess/Board.h>
#include <Chess/Chess.h>
#include <Chess/res/res.h>
#include <QuEngine/Components/QuRenderableComp.h>
#include <QuEngine/Managers/QuInputManager.h>
#include <QuEngine/Managers/QuResourceManager.h>
#include <QuEngine/QuCamera.h>
#include <QuEngine/QuSound.h>
#include <QuEngine/QuTransform.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_int4.hpp>
#include <imgui.h>

using namespace std::string_literals;

Board::Board()
{
  QuInputManager::Instance().BindMousePress(1, "MouseLeft");
  QuInputManager::Instance().BindMousePress(3, "MouseRight");

  QuInputManager::Instance().OnAction("MouseLeft", [&](bool isDown) {});
  QuInputManager::Instance().OnAction("MouseRight", [&](bool isDown) {});

  QuSound::SetVolume(-1, 30);
  auto squareTexture =
    QuResourceManager::Instance().GetTexture("img-square", raw_arr(IMG_SQUARE));
  auto piecesTexture =
    QuResourceManager::Instance().GetTexture("img-pieces", raw_arr(IMG_PIECES));
  m_Tiles.emplace("WhiteKing", MakeTile(piecesTexture, { 0, 0 }));
  m_Tiles.emplace("WhiteQueen", MakeTile(piecesTexture, { 1, 0 }));
  m_Tiles.emplace("WhiteBishop", MakeTile(piecesTexture, { 2, 0 }));
  m_Tiles.emplace("WhiteKnight", MakeTile(piecesTexture, { 3, 0 }));
  m_Tiles.emplace("WhiteRook", MakeTile(piecesTexture, { 4, 0 }));
  m_Tiles.emplace("WhitePawn", MakeTile(piecesTexture, { 5, 0 }));
  m_Tiles.emplace("BlackKing", MakeTile(piecesTexture, { 0, 1 }));
  m_Tiles.emplace("BlackQueen", MakeTile(piecesTexture, { 1, 1 }));
  m_Tiles.emplace("BlackBishop", MakeTile(piecesTexture, { 2, 1 }));
  m_Tiles.emplace("BlackKnight", MakeTile(piecesTexture, { 3, 1 }));
  m_Tiles.emplace("BlackRook", MakeTile(piecesTexture, { 4, 1 }));
  m_Tiles.emplace("BlackPawn", MakeTile(piecesTexture, { 5, 1 }));

  m_Camera = Chess::s_Registry.create();
  Chess::s_Registry.emplace<QuCamera>(m_Camera);
  Chess::s_Registry.emplace<QuTransform>(m_Camera).SetLocalPosition(
    { 0, -200 });

  m_Selector = Chess::s_Registry.create();
  Chess::s_Registry.emplace<QuTransform>(m_Selector)
    .SetLocalPosition({ -100, 0 });
  Chess::s_Registry.emplace<QuRenderableComp>(
    m_Selector, std::make_shared<QuSprite>(squareTexture));

  MakeBoard();
}

void
Board::Render()
{
}

QuSprite
Board::MakeTile(std::shared_ptr<QuTexture> texture, glm::ivec2 position)
{
  auto tile = QuSprite(texture);
  tile.SetRect({ position.x * TILE_SIZE.x,
                 position.y * TILE_SIZE.y,
                 TILE_SIZE.x,
                 TILE_SIZE.y });
  tile.SetSize(PIECE_SIZE);
  return tile;
}

entt::entity
Board::MakePiece(std::string name)
{
  auto piece = Chess::s_Registry.create();
  Chess::s_Registry.emplace<Piece>(piece);
  Chess::s_Registry.emplace<QuTransform>(piece);
  Chess::s_Registry.emplace<QuRenderableComp>(
    piece, std::make_shared<QuSprite>(m_Tiles.at(name)));
  return piece;
}

void
Board::MakeBoard()
{
  auto boardSprite = std::make_shared<QuSprite>(
    QuResourceManager::Instance().GetTexture("img-board", raw_arr(IMG_BOARD)));
  boardSprite->SetSize(PIECE_SIZE * BOARD_SIZE);

  auto board = Chess::s_Registry.create();
  Chess::s_Registry.emplace<QuTransform>(board);
  Chess::s_Registry.emplace<QuRenderableComp>(board, std::move(boardSprite));
}

void
Board::PopulateBoard()
{
  ClearBoard();

  auto whiteKing = MakePiece("WhiteKing");
  auto whiteQueen = MakePiece("WhiteQueen");
  auto whiteBishop = MakePiece("WhiteBishop");
  auto whiteKnight = MakePiece("WhiteKnight");
  auto whiteRook = MakePiece("WhiteRook");

  auto blackKing = MakePiece("BlackKing");
  auto blackQueen = MakePiece("BlackQueen");
  auto blackBishop = MakePiece("BlackBishop");
  auto blackKnight = MakePiece("BlackKnight");
  auto blackRook = MakePiece("BlackRook");
}

void
Board::ClearBoard()
{
  for (auto& piece : m_Board)
    piece = std::nullopt;

  for (auto piece : Chess::s_Registry.view<Piece>())
    Chess::s_Registry.destroy(piece);
}

void
Board::ShowBoard()
{
}

std::optional<entt::entity>
Board::GetHoveredPiece()
{
  return std::nullopt;
}