/*
 * Copyright (C) 2017-2018 by Bennet Carstensen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KINDPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <soGLFWSurface.h>

#include <soVkGLFWSurface.hpp>

char const*
soVkGLFWSurfaceInitialize(void** surface)
{
  try
  {
    *surface = static_cast<void*>(new so::vk::GLFWSurface());
  }
  catch(so::Exception<std::runtime_error> const& e)
  {
    *surface = nullptr;

    return e.what();
  }

  return nullptr;
}

void
soVkGLFWSurfaceTerminate(void* surface)
{
  delete static_cast<so::vk::GLFWSurface*>(surface);
}

char const*
soVkGLFWGetInstanceExtensions(void const*    surface,
                              char const***  extensions,
                              so::size_type* count)
{
  try
  {
    *extensions = static_cast<so::vk::GLFWSurface const*>
                    (surface)->getInstanceExtensions(count);
  }
  catch(so::Exception<std::runtime_error> const& e)
  {
    *extensions = nullptr;

    return e.what();
  }

  return nullptr;
}

so::return_t
soVkGLFWSurfaceCreateWindow(void*                surface,
                            std::string   const& title,
                            so::size_type const  width,
                            so::size_type const  height)
{
  return static_cast<so::vk::GLFWSurface*>(surface)->createWindow(title,
                                                                  width,
                                                                  height);
}

so::return_t
soVkGLFWSurfaceCreateSurface(void* surface, VkInstance instance)
{
  return static_cast<so::vk::GLFWSurface*>(surface)->createSurface(instance);
}

VkSurfaceKHR
soVkGLFWSurfaceGetVkSurfaceKHR(void const* surface)
{
  return static_cast<so::vk::GLFWSurface const*>(surface)->getVkSurfaceKHR();
}

bool
soVkGLFWSurfaceWindowIsClosed(void* surface)
{
  return static_cast<bool>
           (static_cast<so::vk::GLFWSurface*>(surface)->windowIsClosed());
}

void
soVkGLFWSurfacePollEvents()
{
  glfwPollEvents(); 
}

void
soGLFWSurfaceGetWindowSize(void*          surface,
                           so::size_type* width,
                           so::size_type* height)
{
  static_cast<so::vk::GLFWSurface*>(surface)->getWindowSize(*width, *height);
}
