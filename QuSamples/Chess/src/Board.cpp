#include <Chess/Board.h>
#include <Chess/Chess.h>
#include <Chess/chess-engine/chess.hpp>
#include <Chess/res/res.h>
#include <QuEngine/Components/QuRenderableComp.h>
#include <QuEngine/Managers/QuInputManager.h>
#include <QuEngine/Managers/QuResourceManager.h>
#include <QuEngine/QuCamera.h>
#include <QuEngine/QuSound.h>
#include <QuEngine/QuTransform.h>
#include <glm/ext/vector_int4.hpp>
#include <imgui.h>

using namespace std::string_literals;

Board::Board()
{
  QuInputManager::Instance().BindMousePress(1, "MouseLeft");
  QuInputManager::Instance().BindMousePress(3, "MouseRight");

  QuInputManager::Instance().OnAction("MouseLeft", [&](bool isDown) {
    if (!isDown)
      return;

    for (auto selector : Chess::s_Registry.view<Selector>())
      Chess::s_Registry.destroy(selector);

    FindLegalMoves(GetHoveredPiece());
    ShowIndicators();
  });
  QuInputManager::Instance().OnAction("MouseRight", [&](bool isDown) {
    if (!isDown)
      return;

    auto mousePosition = QuInputManager::GetMouseWorldPosition(
      Chess::s_Registry.get<QuTransform>(m_Camera));
    auto selectorPosition =
      glm::vec2(mousePosition.x / PIECE_SIZE.x * PIECE_SIZE.x,
                mousePosition.y / PIECE_SIZE.y * PIECE_SIZE.y);
    for (auto selector : Chess::s_Registry.view<Selector>()) {
      if (Chess::s_Registry.get<QuTransform>(selector).GetWorldPosition() ==
          selectorPosition) {
        Chess::s_Registry.destroy(selector);
        return;
      }
    }

    auto selector = Chess::s_Registry.create();
    Chess::s_Registry.emplace<Selector>(selector);
    Chess::s_Registry.emplace<QuTransform>(selector).SetLocalPosition(
      selectorPosition);
    Chess::s_Registry.emplace<QuRenderableComp>(
      selector,
      std::make_shared<QuSprite>(QuResourceManager::Instance().GetTexture(
        "img-square", raw_arr(IMG_SQUARE))));
  });

  QuSound::SetVolume(-1, 30);
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

  MakeBoard();
  PopulateBoard();
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

void
Board::MakeBoard()
{
  auto boardSprite = std::make_shared<QuSprite>(
    QuResourceManager::Instance().GetTexture("img-board", raw_arr(IMG_BOARD)));

  auto board = Chess::s_Registry.create();
  Chess::s_Registry.emplace<QuTransform>(board);
  Chess::s_Registry.emplace<QuRenderableComp>(board, std::move(boardSprite));
}

void
Board::PopulateBoard()
{
  for (auto& piece : m_Board)
    piece = std::nullopt;
  for (auto i = 0; i < BOARD_SIZE.x; i++)
    m_Board[idx(i, 1)] = Color::White | Type::Pawn;
  for (auto i = 0; i < BOARD_SIZE.x; i++)
    m_Board[idx(i, 6)] = Color::Black | Type::Pawn;
  m_Board[idx(4, 0)] = Color::White | Type::King;
  m_Board[idx(3, 0)] = Color::White | Type::Queen;
  m_Board[idx(2, 0)] = Color::White | Type::Bishop;
  m_Board[idx(5, 0)] = Color::White | Type::Bishop;
  m_Board[idx(1, 0)] = Color::White | Type::Knight;
  m_Board[idx(6, 0)] = Color::White | Type::Knight;
  m_Board[idx(0, 0)] = Color::White | Type::Rook;
  m_Board[idx(7, 0)] = Color::White | Type::Rook;
  m_Board[idx(4, 7)] = Color::Black | Type::King;
  m_Board[idx(3, 7)] = Color::Black | Type::Queen;
  m_Board[idx(2, 7)] = Color::Black | Type::Bishop;
  m_Board[idx(5, 7)] = Color::Black | Type::Bishop;
  m_Board[idx(1, 7)] = Color::Black | Type::Knight;
  m_Board[idx(6, 7)] = Color::Black | Type::Knight;
  m_Board[idx(0, 7)] = Color::Black | Type::Rook;
  m_Board[idx(7, 7)] = Color::Black | Type::Rook;
  ShowBoard();
}

void
Board::ClearBoard()
{
  for (auto& piece : m_Board)
    piece = std::nullopt;
  ShowBoard();
}

void
Board::ShowBoard()
{
  for (auto piece : Chess::s_Registry.view<Piece>())
    Chess::s_Registry.destroy(piece);

  for (auto i = 0; i < m_Board.size(); i++) {
    auto& piece = m_Board.at(i);
    if (!piece.has_value())
      continue;

    auto name = *piece & Color::Black ? "Black"s : "White";
    auto type = *piece & Type::Queen ? "Queen" : "King";
    type = *piece & Type::Bishop ? "Bishop" : type;
    type = *piece & Type::Knight ? "Knight" : type;
    type = *piece & Type::Rook ? "Rook" : type;
    type = *piece & Type::Pawn ? "Pawn" : type;
    name += type;

    auto entity = Chess::s_Registry.create();
    Chess::s_Registry.emplace<Piece>(entity);
    Chess::s_Registry.emplace<QuTransform>(entity).SetLocalPosition(
      checked(coord(i)) * PIECE_SIZE);
    Chess::s_Registry.emplace<QuRenderableComp>(
      entity, std::make_shared<QuSprite>(m_Tiles.at(name)));
  }
}

void
Board::FindLegalMoves(std::optional<entt::entity> piece)
{
  m_LegalMoves = {};
  if (!piece.has_value())
    return;

  auto pos = Chess::s_Registry.get<QuTransform>(*piece).GetWorldPosition();
  auto square = checked(glm::ivec2(pos.x / PIECE_SIZE.x, pos.y / PIECE_SIZE.y));

  auto legalMoves = std::vector<uint8_t>();
  auto fenColor = m_Color == Color::White ? "w"s : "b";
  auto board = chess::Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR " +
                            fenColor + " KQkq - 0 1");
  auto moves = chess::Movelist();
  chess::movegen::legalmoves(moves, board);
  for (auto& move : moves)
    if (move.from() == idx(square.x, square.y))
      legalMoves.push_back(move.to());

  if (!legalMoves.empty())
    m_LegalMoves = { piece, legalMoves };
}

void
Board::ShowIndicators()
{
  for (auto indicator : Chess::s_Registry.view<Indicator>())
    Chess::s_Registry.destroy(indicator);
  for (auto& move : m_LegalMoves.second) {
    auto indicator = Chess::s_Registry.create();
    Chess::s_Registry.emplace<Indicator>(indicator);
    Chess::s_Registry.emplace<QuTransform>(indicator).SetLocalPosition(
      checked(coord(move)) * PIECE_SIZE);
    Chess::s_Registry.emplace<QuRenderableComp>(
      indicator,
      std::make_shared<QuSprite>(QuResourceManager::Instance().GetTexture(
        "img-circle", raw_arr(IMG_CIRCLE))));
  }
}

std::optional<entt::entity>
Board::GetHoveredPiece()
{
  auto mousePosition = QuInputManager::GetMouseWorldPosition(
    Chess::s_Registry.get<QuTransform>(m_Camera));
  auto cells = Chess::s_Registry.view<Piece, QuTransform>();
  for (auto cell : cells) {
    auto cellPosition = cells.get<QuTransform>(cell).GetWorldPosition();
    if (mousePosition.x >= cellPosition.x &&
        mousePosition.y >= cellPosition.y &&
        mousePosition.x < cellPosition.x + PIECE_SIZE.x &&
        mousePosition.y < cellPosition.y + PIECE_SIZE.y)
      return cell;
  }
  return std::nullopt;
}