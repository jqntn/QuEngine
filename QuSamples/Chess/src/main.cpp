#include <Chess/Chess.h>
#include <QuEngine/QuEngine.h>
#include <Windows.h>

#ifdef _DEBUG
int
main()
#else
int WINAPI
wWinMain(_In_ HINSTANCE hInstance,
         _In_opt_ HINSTANCE hPrevInstance,
         _In_ PWSTR pCmdLine,
         _In_ int nCmdShow)
#endif
{
#ifdef _DEBUG
  Chess chess;
#else
  SaveLogs();
  try {
    Chess chess;
  } catch (const std::exception& e) {
    LogError(e.what());
  }
#endif
}