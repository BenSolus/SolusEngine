
#include <soEngine.h>

int
main() 
{
  try
  {  
    so::Engine engine
      ("Hello GLFWwindow", VK_MAKE_VERSION(0, 0, 1));

    while(not engine.windowIsClosed())
      engine.surfacePollEvents();
  
  }
  catch(so::Exception<std::runtime_error> const& e)
  {
    std::cerr << "Unexpected error: " << PRETTY_FUNCTION_SIG << "\n";
    e.print();

   return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

