
#include <soEngine.hpp>

#include <iostream>

int
main() 
{
  std::cout << "Hello World!\n";

  so::Engine<so::EngineBackend::Vulkan, so::SurfaceBackend::GLFW> engine;

  engine.createSurface("GLFWwindow");

  while(not engine.surfaceIsClosed())
  {
    engine.surfacePollEvents();
  }

  return EXIT_SUCCESS;
}

