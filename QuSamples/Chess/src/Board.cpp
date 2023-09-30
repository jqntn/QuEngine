#include <Chess/Board.h>
#include <Chess/Chess.h>
#include <Chess/Client.h>
#include <Chess/res/res.h>
#include <Chess/utils/magic_enum.hpp>
#include <QuEngine/Components/QuRenderableComp.h>
#include <QuEngine/Managers/QuInputManager.h>
#include <QuEngine/Managers/QuResourceManager.h>
#include <QuEngine/QuCamera.h>
#include <QuEngine/QuSound.h>
#include <QuEngine/QuTransform.h>
#include <imgui.h>

using namespace std::string_literals;

Board::Board()
{
  QuInputManager::Instance().BindMousePress(1, "MouseLeft");
  QuInputManager::Instance().BindMousePress(3, "MouseRight");

  QuInputManager::Instance().OnAction("MouseLeft", [&](bool isDown) {
    if (!isDown)
      return;

    if (m_pClient->Room.UID != "Local" &&
        (m_pClient->Room.P1.Color != m_Board.sideToMove() ||
         m_pClient->Room.P2.UID.empty()))
      return;

    for (auto selector : Chess::s_Registry.view<Selector>())
      Chess::s_Registry.destroy(selector);

    auto isWindowHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
    if (!isWindowHovered && m_LegalMoves.first.has_value())
      for (const auto& move : m_LegalMoves.second)
        if (cr(crd(move.to())) * PIECE_SIZE == GetHoveredSquare())
          m_pClient->SendMove(move);

    auto hoveredPiece = isWindowHovered ? std::nullopt : GetHoveredPiece();
    FindLegalMoves(hoveredPiece);
    ShowIndicators();
  });
  QuInputManager::Instance().OnAction("MouseRight", [&](bool isDown) {
    if (!isDown)
      return;

    auto square = glm::vec2(GetHoveredSquare());
    for (auto selector : Chess::s_Registry.view<Selector>())
      if (Chess::s_Registry.get<QuTransform>(selector).GetWorldPosition() ==
          square) {
        Chess::s_Registry.destroy(selector);
        return;
      }

    auto selector = Chess::s_Registry.create();
    Chess::s_Registry.emplace<Selector>(selector);
    Chess::s_Registry.emplace<QuTransform>(selector).SetLocalPosition(square);
    Chess::s_Registry.emplace<QuRenderableComp>(
      selector,
      std::make_shared<QuSprite>(QuResourceManager::Instance().GetTexture(
        "img-square", raw_arr(IMG_SQUARE))));
  });

  QuSound::SetVolume(-1, 30);
  auto piecesTexture =
    QuResourceManager::Instance().GetTexture("img-pieces", raw_arr(IMG_PIECES));
  {
    using C = chess::Color;
    using PT = chess::PieceType;
    m_Tiles.emplace(C::WHITE | PT::KING, MakeTile(piecesTexture, { 0, 0 }));
    m_Tiles.emplace(C::WHITE | PT::QUEEN, MakeTile(piecesTexture, { 1, 0 }));
    m_Tiles.emplace(C::WHITE | PT::BISHOP, MakeTile(piecesTexture, { 2, 0 }));
    m_Tiles.emplace(C::WHITE | PT::KNIGHT, MakeTile(piecesTexture, { 3, 0 }));
    m_Tiles.emplace(C::WHITE | PT::ROOK, MakeTile(piecesTexture, { 4, 0 }));
    m_Tiles.emplace(C::WHITE | PT::PAWN, MakeTile(piecesTexture, { 5, 0 }));
    m_Tiles.emplace(C::BLACK | PT::KING, MakeTile(piecesTexture, { 0, 1 }));
    m_Tiles.emplace(C::BLACK | PT::QUEEN, MakeTile(piecesTexture, { 1, 1 }));
    m_Tiles.emplace(C::BLACK | PT::BISHOP, MakeTile(piecesTexture, { 2, 1 }));
    m_Tiles.emplace(C::BLACK | PT::KNIGHT, MakeTile(piecesTexture, { 3, 1 }));
    m_Tiles.emplace(C::BLACK | PT::ROOK, MakeTile(piecesTexture, { 4, 1 }));
    m_Tiles.emplace(C::BLACK | PT::PAWN, MakeTile(piecesTexture, { 5, 1 }));
  }

  auto board = Chess::s_Registry.create();
  Chess::s_Registry.emplace<QuTransform>(board);
  Chess::s_Registry.emplace<QuRenderableComp>(
    board,
    std::make_shared<QuSprite>(QuResourceManager::Instance().GetTexture(
      "img-board", raw_arr(IMG_BOARD))));

  m_Camera = Chess::s_Registry.create();
  Chess::s_Registry.emplace<QuCamera>(m_Camera);
  Chess::s_Registry.emplace<QuTransform>(m_Camera).SetLocalPosition(
    { 0, -200 });

  m_pClient = new Client();
  m_pClient->RoomReceived = [this]() { m_IsMakeBoardDirty = true; };
  m_pClient->MoveReceived = [this](const chess::Move& move) {
    m_ReceivedMove = move;
  };

  MakeBoard();
}

Board::~Board()
{
  delete m_pClient;
}

void
Board::Tick()
{
  if (m_IsMakeBoardDirty) {
    MakeBoard();
    for (const auto& move : m_pClient->Room.Moves)
      MakeMove(move);
    m_IsMakeBoardDirty = false;
  }

  if (m_ReceivedMove != chess::Move::NO_MOVE) {
    MakeMove(m_ReceivedMove);
    m_ReceivedMove = chess::Move::NO_MOVE;
  }
}

void
Board::RenderPlayer(const Player& player)
{
  if (m_pClient->Room.UID != "Local" && player.UID.empty())
    return;
  auto color = player.Color;
  auto imageBit = color == chess::Color::WHITE ? 1.f : 0;
  auto textBit = m_Board.sideToMove() != color ? 1.f : 0;
  ImGui::TableNextColumn();
  ImGui::Image({}, { 30, 30 }, {}, {}, { imageBit, imageBit, imageBit, 1 });
  ImGui::SameLine();
  ImGui::TextColored({ textBit, 1, textBit, 1 }, player.Name.c_str());
  ImGui::Dummy({ 0, 10 });
  ImGui::Text("Last Move: %s", m_LastMove[color].c_str());
}

void
Board::Render()
{
  ImGui::GetStyle().WindowTitleAlign = { .5f, .5f };
  auto size = ImGui::GetMainViewport()->WorkSize;
  {
    auto flags = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoFocusOnAppearing |
                 ImGuiWindowFlags_NoBringToFrontOnFocus |
                 ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowPos({});
    ImGui::SetNextWindowSize({ size.x, 100 });
    ImGui::Begin(("MENU - Room: " + m_pClient->Room.UID).c_str(), 0, flags);
    {
      ImGui::SetNextItemWidth(80);
      ImGui::InputText("Username",
                       m_pClient->Username.second,
                       sizeof(m_pClient->Username.second),
                       ImGuiInputTextFlags_CharsNoBlank);

      auto showDraw = true;
      if (m_pClient->Room.UID != "Local" &&
          m_Board.sideToMove() != m_pClient->Room.P1.Color)
        showDraw = false;
      if (showDraw) {
        ImGui::SameLine();
        ImGui::SetCursorPosX(size.x - 60);
        if (ImGui::Button(" Draw "))
          m_pClient->SendMove(chess::Move::NULL_MOVE);
      }

      if (ImGui::Button(" Create Room "))
        m_pClient->CreateRoom();
      ImGui::SameLine();
      ImGui::TextColored({ 1, 0, 0, 1 }, m_pClient->LastError.c_str());
      if (ImGui::Button("  Join Room  "))
        m_pClient->JoinRoom();
      ImGui::SameLine();
      ImGui::SetNextItemWidth(60);
      ImGui::InputText("##RoomToJoin",
                       m_pClient->RoomToJoin,
                       sizeof(m_pClient->RoomToJoin),
                       ImGuiInputTextFlags_CharsNoBlank |
                         ImGuiInputTextFlags_AutoSelectAll |
                         ImGuiInputTextFlags_CharsDecimal);
    }
    ImGui::End();

    ImGui::SetNextWindowPos({ 0, 100 });
    ImGui::SetNextWindowSize({ size.x, 100 });
    ImGui::Begin("PLAYERS", 0, flags | ImGuiWindowFlags_NoInputs);
    {
      ImGui::BeginTable("##", 2, ImGuiTableFlags_NoPadInnerX);
      RenderPlayer(m_pClient->Room.P1);
      RenderPlayer(m_pClient->Room.P2);
      ImGui::EndTable();
    }
    ImGui::End();
  }
  {
    if (m_GameState.second == chess::GameResult::NONE)
      return;
    auto result = magic_enum::enum_name(m_GameState.second);
    auto reason = magic_enum::enum_name(m_GameState.first);
    auto state = std::string(result) + " - " + std::string(reason);
    auto isDraw = result == magic_enum::enum_name(chess::GameResult::DRAW);

    auto flags = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoCollapse;
    auto position = ImVec2((size.x - 256) / 2, (size.y + 72) / 2);
    ImGui::SetNextWindowPos(position, ImGuiCond_Once);
    ImGui::SetNextWindowSize({ 256, 128 });
    ImGui::Begin(state.c_str(), 0, flags);
    {
      ImGui::Text("Turn: %i", m_Board.fullMoveNumber());
      ImGui::Dummy({ 0, 50 });

      auto showRematch = true;
      if (isDraw && m_pClient->Room.UID != "Local" &&
          m_Board.sideToMove() != m_pClient->Room.P1.Color)
        showRematch = false;
      if (showRematch &&
          ImGui::Button(
            ("Rematch"s + (isDraw ? " (or play to deny)" : "")).c_str(),
            { ImGui::GetContentRegionAvail().x, 20 }))
        m_pClient->SendMove(chess::MAX_MOVES);
    }
    ImGui::End();
  }
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
  m_Board = chess::Board();
  m_LastMove = {};
  m_GameState = { chess::GameResultReason::NONE, chess::GameResult::NONE };
  ShowBoard();
}

void
Board::ShowBoard()
{
  for (auto piece : Chess::s_Registry.view<Piece>())
    Chess::s_Registry.destroy(piece);
  for (auto i = 0; i < (uint8_t)chess::Color::NONE; i++)
    for (auto j = 0; j < (uint8_t)chess::PieceType::NONE; j++) {
      auto pieces = m_Board.pieces((chess::PieceType)j, (chess::Color)i);
      for (auto _ = 0; _ < chess::builtin::popcount(pieces);) {
        auto piece = chess::builtin::poplsb(pieces);
        auto entity = Chess::s_Registry.create();
        Chess::s_Registry.emplace<Piece>(entity);
        Chess::s_Registry.emplace<QuTransform>(entity).SetLocalPosition(
          cr(crd(piece)) * PIECE_SIZE);
        Chess::s_Registry.emplace<QuRenderableComp>(
          entity,
          std::make_shared<QuSprite>(
            m_Tiles.at((chess::Color)i | (chess::PieceType)j)));
      }
    }
}

void
Board::ShowIndicators()
{
  for (auto indicator : Chess::s_Registry.view<Indicator>())
    Chess::s_Registry.destroy(indicator);
  for (const auto& move : m_LegalMoves.second) {
    auto indicator = Chess::s_Registry.create();
    Chess::s_Registry.emplace<Indicator>(indicator);
    Chess::s_Registry.emplace<QuTransform>(indicator).SetLocalPosition(
      cr(crd(move.to())) * PIECE_SIZE);
    Chess::s_Registry.emplace<QuRenderableComp>(
      indicator,
      std::make_shared<QuSprite>(QuResourceManager::Instance().GetTexture(
        "img-circle", raw_arr(IMG_CIRCLE))));
  }
}

void
Board::FindLegalMoves(std::optional<entt::entity> piece)
{
  m_LegalMoves = {};
  if (!piece.has_value())
    return;
  m_LegalMoves.first = piece;

  auto position = Chess::s_Registry.get<QuTransform>(*piece).GetWorldPosition();
  auto square =
    cr(glm::ivec2(position.x / PIECE_SIZE.x, position.y / PIECE_SIZE.y));
  auto moves = chess::Movelist();
  chess::movegen::legalmoves(moves, m_Board);
  for (const auto& move : moves)
    if (move.from() == idx(square.x, square.y))
      m_LegalMoves.second.push_back(move);
}

void
Board::MakeMove(const chess::Move& move)
{
  if (move == chess::Move::NULL_MOVE) {
    m_Board.makeNullMove();
    m_GameState = { chess::GameResultReason::NONE, chess::GameResult::DRAW };
  } else if (move == chess::MAX_MOVES) {
    if (!m_IsMakeBoardDirty)
      std::swap(m_pClient->Room.P1.Color, m_pClient->Room.P2.Color);
    MakeBoard();
  } else {
    m_LastMove[m_Board.sideToMove()] = chess::uci::moveToLan(m_Board, move);
    m_Board.makeMove(move);
    m_GameState = m_Board.isGameOver();
    ShowBoard();
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

glm::ivec2
Board::GetHoveredSquare()
{
  auto mousePosition = QuInputManager::GetMouseWorldPosition(
    Chess::s_Registry.get<QuTransform>(m_Camera));
  return glm::vec2(mousePosition.x / PIECE_SIZE.x * PIECE_SIZE.x,
                   mousePosition.y / PIECE_SIZE.y * PIECE_SIZE.y);
}