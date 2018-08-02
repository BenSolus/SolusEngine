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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

/**
 *  @file      soGLFWSurfaceInterface.hpp
 *  @author    Bennet Carstensen
 *  @date      2018
 *  @copyright Copyright (c) 2017-2018 Bennet Carstensen
 *
 *             Permission is hereby granted, free of charge, to any person
 *             obtaining a copy of this software and associated documentation
 *             files (the "Software"), to deal in the Software without
 *             restriction, including without limitation the rights to use,
 *             copy, modify, merge, publish, distribute, sublicense, and/or
 *             sell copies of the Software, and to permit persons to whom the
 *             Software is furnished to do so, subject to the following
 *             conditions:
 *
 *             The above copyright notice and this permission notice shall be
 *             included in all copies or substantial portions of the Software.
 *
 *             THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *             EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *             OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *             NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *             HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *             WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *             FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *             OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <interfaces/soSurfaceInterface.hpp>

#include <soDefinitions.hpp>
#include <soException.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace so {
    
template<>
class
SurfaceInterface<SurfaceBackend::GLFW>
{
  public:
    SurfaceInterface() : mWindow(nullptr) { }
    
    SurfaceInterface(std::string const& title) : SurfaceInterface()
    {
      glfwInit();

      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
      glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

      mWindow = glfwCreateWindow(800,
                                 600,
                                 title.c_str(),
                                 nullptr,
                                 nullptr);

      if(mWindow is_eq nullptr)
        throw Exception<std::runtime_error>("Failed to create GLFWwindow.",
                                            PRETTY_FUNCTION_SIG);
    }

    SurfaceInterface(SurfaceInterface const& other) = delete;

    SurfaceInterface(SurfaceInterface&& other) = delete;

    ~SurfaceInterface() noexcept { deleteMembers(); }

    SurfaceInterface&
    operator=(SurfaceInterface const& other) = delete;

    SurfaceInterface&
    operator=(SurfaceInterface&& other) noexcept
    {
      if(this is_eq &other)
        return *this;

      deleteMembers();

      mWindow = other.mWindow;

      other.mWindow = nullptr;

      return *this;
    }

    inline auto isClosed() { return glfwWindowShouldClose(mWindow); }
      
    inline void pollEvents() { glfwPollEvents(); }

  protected:
    GLFWwindow* mWindow;

  private:
    void
    deleteMembers()
    {
      if(mWindow not_eq nullptr)
      {
        glfwDestroyWindow(mWindow);

        glfwTerminate();

        mWindow = nullptr;
      }
    }
}; // class SurfaceInterface

} // namespace so