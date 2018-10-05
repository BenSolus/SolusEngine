
#include "soEngine.h"

#include "cxx/soDebugCallback.hpp"

int
main() 
{ 
  so::setDebugCallback([](so::DebugCode const code, std::string const& message)
                       {
                         std::string debugMessage{ message };
                         
                         switch(code)
                         {
                           case so::DebugCode::info:
                             debugMessage.insert(0, "<INFO>    ");
                             break;
                           case infoItem:
                             debugMessage.insert(0, "<INFO>      ");
                             break;
                           case so::DebugCode::verbose:
                             debugMessage.insert(0, "<VERBOSE> ");
                             break;
                           case so::DebugCode::error:
                             debugMessage.insert(0, "<ERROR>   ");
                             break;
                           default:
                             break;
                         }

                         puts(debugMessage.c_str());
                       });

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

