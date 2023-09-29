#define ENET_IMPLEMENTATION

#include "ChessServer.h"
#include "enet.h"

int
main()
{
  if (enet_initialize() < 0) {
    fprintf(stderr, "An error occurred while initializing ENet.\n");
    return EXIT_FAILURE;
  }
  atexit(enet_deinitialize);

  std::cout << "Hello CMake." << std::endl;
  return EXIT_SUCCESS;
}