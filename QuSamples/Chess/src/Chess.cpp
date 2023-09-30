#include <Chess/Board.h>
#include <Chess/Chess.h>
#include <QuEngine/QuEngine.h>
#include <QuEngine/QuEvent.h>
#include <QuEngine/QuGUI.h>
#include <QuEngine/QuRenderer2D.h>
#include <QuEngine/QuWindow.h>
#include <QuEngine/Systems/QuRenderSystem.h>
#include <glm/ext/vector_uint4.hpp>

Chess::Chess()
{
  auto quEngine = new QuEngine();
  auto quWindow = new QuWindow("Chess", { 512, 712 });
  auto quRenderer = new QuRenderer2D(*quWindow);
  auto quGUI = new QuGUI(*quWindow, *quRenderer);
  auto quEvent = new QuEvent();
  auto quRenderSystem = new QuRenderSystem(s_Registry);
  auto pBoard = new Board();

  quRenderer->SetClearColor({ 50, 50, 50, 50 });
  quEngine->StartCounters();
  while (true) {
    if (quEngine->PollEvent(*quEvent)) {
      if (quEvent->IsQuit())
        break;
      quGUI->HandleEvent(*quEvent);
    }
    quEngine->UpdateCounters();

    pBoard->Tick();

    quRenderer->Clear();
    quGUI->NewFrame();
    pBoard->Render();
    quRenderSystem->Run();
    quGUI->Render();
    quRenderer->Present();
  }

  delete pBoard;
  delete quRenderSystem;
  delete quEvent;
  delete quGUI;
  delete quRenderer;
  delete quWindow;
  delete quEngine;
}