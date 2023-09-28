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

  QuInputManager::Instance().OnAction("MouseLeft", [&](bool isDown) {
    if (!isDown)
      return;
    auto hoveredCell = GetHoveredCell();
    if (!hoveredCell.has_value())
      return;
    UnveilCell(hoveredCell.value());
  });
  QuInputManager::Instance().OnAction("MouseRight", [&](bool isDown) {
    if (!isDown)
      return;
    auto hoveredCell = GetHoveredCell();
    if (!hoveredCell.has_value())
      return;
    FlagCell(hoveredCell.value());
  });

  QuSound::SetVolume(-1, 30);
  auto mainTexture = QuResourceManager::Instance().GetTexture(
    "img-minesweeper", raw_arr(IMG_MINESWEEPER));
  m_Tiles.emplace("1", MakeTile(mainTexture, { 0, 1 }));
  m_Tiles.emplace("2", MakeTile(mainTexture, { 1, 1 }));
  m_Tiles.emplace("3", MakeTile(mainTexture, { 2, 1 }));
  m_Tiles.emplace("4", MakeTile(mainTexture, { 3, 1 }));
  m_Tiles.emplace("5", MakeTile(mainTexture, { 4, 1 }));
  m_Tiles.emplace("6", MakeTile(mainTexture, { 5, 1 }));
  m_Tiles.emplace("7", MakeTile(mainTexture, { 6, 1 }));
  m_Tiles.emplace("8", MakeTile(mainTexture, { 7, 1 }));
  m_Tiles.emplace("Flag", MakeTile(mainTexture, { 2, 0 }));
  m_Tiles.emplace("Bomb", MakeTile(mainTexture, { 6, 0 }));
  m_Tiles.emplace("Background", MakeTile(mainTexture, { 1, 0 }));
  m_Tiles.emplace("Foreground", MakeTile(mainTexture, { 0, 0 }));

  m_Camera = Minesweeper::s_Registry.create();
  Minesweeper::s_Registry.emplace<QuCamera>(m_Camera);
  Minesweeper::s_Registry.emplace<QuTransform>(m_Camera).SetLocalPosition(
    { 0, -100 });

  MakeGrid();
}

void
Board::Render()
{
  auto windowSize = ImGui::GetMainViewport()->WorkSize;

  ImGui::SetNextWindowPos({});
  ImGui::SetNextWindowSize({ windowSize.x, 100 });
  ImGui::Begin(
    "##", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
  {
    ImGui::BeginTable("##", 3);
    {
      ImGui::TableNextColumn();
      ImGui::Text("MouseLeft: Unveil");
      ImGui::Text("MouseRight: Flag");
      ImGui::Dummy({ 0, 30 });
      ImGui::Text(("Flags: " + std::to_string(m_FlagCount)).c_str());
    }
    {
      ImGui::TableNextColumn();
      ImGui::SetCursorPos({ (windowSize.x - 50) / 2.f, (100 - 50) / 2.f });
      if (ImGui::Button("Retry", { 50, 50 }))
        MakeGrid();
    }
    {
      ImGui::TableNextColumn();
      ImGui::Dummy({ 0, 40 });
      ImGui::SetCursorPosX(windowSize.x -
                           ImGui::CalcTextSize("Difficulty (0-1): ").x);
      ImGui::Text("Difficulty (0-1): ");
      ImGui::SetCursorPosX(windowSize.x - 100);
      ImGui::InputFloat("##", &m_BombProba);
    }
    ImGui::EndTable();
  }
  ImGui::End();
}

QuSprite
Board::MakeTile(std::shared_ptr<QuTexture> texture, glm::ivec2 position)
{
  auto tile = QuSprite(texture);
  tile.SetRect({ position.x * TILE_SIZE.x,
                 position.y * TILE_SIZE.y,
                 TILE_SIZE.x,
                 TILE_SIZE.y });
  tile.SetSize(CELL_SIZE);
  return tile;
}

void
Board::MakeGrid()
{
  m_IsGridPopulated = false;
  m_FlagCount = 0;
  for (auto cell : Minesweeper::s_Registry.view<Cell>())
    Minesweeper::s_Registry.destroy(cell);
  for (auto y = 0; y < GRID_SIZE.y; y++) {
    for (auto x = 0; x < GRID_SIZE.x; x++) {
      auto cell = Minesweeper::s_Registry.create();
      Minesweeper::s_Registry.emplace<Cell>(cell);
      Minesweeper::s_Registry.emplace<QuTransform>(cell).SetLocalPosition(
        { x * CELL_SIZE.x, y * CELL_SIZE.y });
      Minesweeper::s_Registry.emplace<QuRenderableComp>(
        cell, std::make_shared<QuSprite>(m_Tiles.at("Foreground")));
    }
  }
}

void
Board::PopulateGrid()
{
  std::srand((uint32_t)std::time(nullptr));

  auto bombs = std::vector<entt::entity>();
  auto cells = Minesweeper::s_Registry.view<Cell>();
  for (auto cell : cells) {
    auto& cellData = cells.get<Cell>(cell);
    if (cellData.State == Cell::State::Reserved) {
      cellData.State = Cell::State::Veiled;
    } else if (std::rand() / (float)RAND_MAX < m_BombProba) {
      cellData.Type = Cell::Type::Bomb;
      bombs.push_back(cell);
    }
  }

  for (auto bomb : bombs) {
    for (auto adjacentCell : GetAdjacentCells(bomb)) {
      auto& adjacentCellData = cells.get<Cell>(adjacentCell);
      if (adjacentCellData.Type != Cell::Type::Bomb) {
        adjacentCellData.Type = Cell::Type::Number;
        adjacentCellData.BombCount++;
      }
    }
  }

  m_FlagCount = bombs.size();
  m_IsGridPopulated = true;
}

std::optional<entt::entity>
Board::GetHoveredCell()
{
  auto mousePosition = QuInputManager::GetMousePosition();
  auto mouseVector = glm::ivec4(mousePosition.x, mousePosition.y, 0, 1);

  auto& cameraTransform = Minesweeper::s_Registry.get<QuTransform>(m_Camera);
  mouseVector = cameraTransform.GetTransformMatrix() * mouseVector;

  auto cells = Minesweeper::s_Registry.view<Cell, QuTransform>();
  for (auto cell : cells) {
    auto cellPosition = cells.get<QuTransform>(cell).GetWorldPosition();
    if (mouseVector.x >= cellPosition.x && mouseVector.y >= cellPosition.y &&
        mouseVector.x < cellPosition.x + CELL_SIZE.x &&
        mouseVector.y < cellPosition.y + CELL_SIZE.y)
      return cell;
  }
  return std::nullopt;
}

std::vector<entt::entity>
Board::GetAdjacentCells(entt::entity centerCell)
{
  auto centerPosition =
    Minesweeper::s_Registry.get<QuTransform>(centerCell).GetLocalPosition();
  auto adjacentCells = std::vector<entt::entity>();
  auto cells = Minesweeper::s_Registry.view<Cell, QuTransform>();
  for (auto cell : cells) {
    auto cellPosition = cells.get<QuTransform>(cell).GetLocalPosition();
    if ((std::abs(cellPosition.x - centerPosition.x) == 0 ||
         std::abs(cellPosition.x - centerPosition.x) == CELL_SIZE.x) &&
        (std::abs(cellPosition.y - centerPosition.y) == 0 ||
         std::abs(cellPosition.y - centerPosition.y) == CELL_SIZE.y))
      adjacentCells.push_back(cell);
  }
  return adjacentCells;
}

void
Board::UnveilCell(entt::entity cell)
{
  if (!m_IsGridPopulated) {
    for (auto adjacentCell : GetAdjacentCells(cell))
      Minesweeper::s_Registry.get<Cell>(adjacentCell).State =
        Cell::State::Reserved;
    PopulateGrid();
    UnveilCell(cell);
    return;
  }

  auto& cellData = Minesweeper::s_Registry.get<Cell>(cell);
  if (cellData.State != Cell::State::Veiled)
    return;
  cellData.State = Cell::State::Unveiled;

  auto tile = "Background"s;
  switch (cellData.Type) {
    case Cell::Type::Empty: {
      for (auto adjacentCell : GetAdjacentCells(cell))
        UnveilCell(adjacentCell);
    } break;
    case Cell::Type::Number: {
      tile = std::to_string(cellData.BombCount);
    } break;
    case Cell::Type::Bomb: {
      tile = "Bomb";
      QuResourceManager::Instance()
        .GetSound("sfx-vineboom", raw_arr(SFX_VINEBOOM))
        ->Play();
    } break;
  }

  Minesweeper::s_Registry.replace<QuRenderableComp>(
    cell, std::make_shared<QuSprite>(m_Tiles.at(tile)));
}

void
Board::FlagCell(entt::entity cell)
{
  auto& cellData = Minesweeper::s_Registry.get<Cell>(cell);

  auto tile = "Flag"s;
  switch (cellData.State) {
    case Cell::State::Veiled: {
      if (m_FlagCount <= 0)
        return;
      cellData.State = Cell::State::Flagged;
      m_FlagCount--;
      QuResourceManager::Instance()
        .GetSound("sfx-tacobell", raw_arr(SFX_TACOBELL))
        ->Play();
    } break;
    case Cell::State::Flagged: {
      cellData.State = Cell::State::Veiled;
      m_FlagCount++;
      tile = "Foreground";
    } break;
    default:
      return;
  }

  Minesweeper::s_Registry.replace<QuRenderableComp>(
    cell, std::make_shared<QuSprite>(m_Tiles.at(tile)));
}