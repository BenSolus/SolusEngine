
#include "soEngine.h"

int
main() 
{  
  so::Engine engine;

  so::return_t const result(engine.initialize("Hello GLFWwindow",
                                              VK_MAKE_VERSION(0, 0, 1)));

  if(result == failure)
  {
    return EXIT_FAILURE;
  }

  while(not engine.windowIsClosed())
  {
    engine.surfacePollEvents();
  }

  return EXIT_SUCCESS;
}

