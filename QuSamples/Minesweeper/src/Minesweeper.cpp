#include <Minesweeper/Board.h>
#include <Minesweeper/Minesweeper.h>
#include <QuEngine/QuEngine.h>
#include <QuEngine/QuEvent.h>
#include <QuEngine/QuGUI.h>
#include <QuEngine/QuRenderer2D.h>
#include <QuEngine/QuWindow.h>
#include <QuEngine/Systems/QuRenderSystem.h>
#include <glm/ext/vector_uint4.hpp>

Minesweeper::Minesweeper()
{
  auto quEngine = new QuEngine();
  auto quWindow = new QuWindow("Minesweeper", { 512, 612 });
  auto quRenderer = new QuRenderer2D(*quWindow);
  auto quGUI = new QuGUI(*quWindow, *quRenderer);
  auto quEvent = new QuEvent();
  auto quRenderSystem = QuRenderSystem(s_Registry);
  auto board = Board();

  quRenderer->SetClearColor({ 50, 50, 50, 50 });
  quEngine->StartCounters();
  while (true) {
    if (quEngine->PollEvent(*quEvent)) {
      if (quEvent->IsQuit())
        break;
      quGUI->HandleEvent(*quEvent);
    }
    quEngine->UpdateCounters();

    quRenderer->Clear();
    quGUI->NewFrame();
    board.Render();
    quRenderSystem.Run();
    quGUI->Render();
    quRenderer->Present();
  }

  delete quEvent;
  delete quGUI;
  delete quRenderer;
  delete quWindow;
  delete quEngine;
}