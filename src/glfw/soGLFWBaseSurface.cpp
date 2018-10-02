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

#include "soGLFWBaseSurface.hpp"

#include <cstdio>

void error_callback(int error, char const* description);

void error_callback(int error, char const* description)
{ 
  (void) error;
  puts(description);
}


so::base::Surface::Surface() : mIsInitialized(false), mWindow(nullptr)
{
}

so::base::Surface::~Surface() noexcept
{
  deleteMembers();

  if(mIsInitialized)
  {
    glfwTerminate();
  }
}

so::base::Surface&
so::base::Surface::operator=(Surface&& other) noexcept
{
  if(this is_eq &other)
  {
    return *this;
  }

  deleteMembers();

  mIsInitialized = other.mIsInitialized;
  mWindow        = other.mWindow;

  other.mIsInitialized = false;
  other.mWindow        = nullptr;

  return *this;
}

so::return_t
so::base::Surface::initialize()
{
  glfwSetErrorCallback(error_callback);

  if(glfwInit() is_eq GLFW_FALSE)
  {
    return failure;
  }

  mIsInitialized = true;

  return success;
}

void
so::base::Surface::getWindowSize(size_type& width, size_type& height)
{
  int iWidth{ -1 };
  int iHeight{ -1 };

  glfwGetWindowSize(mWindow, &iWidth, &iHeight);

  width  = static_cast<size_type>(iWidth);
  height = static_cast<size_type>(iHeight);
}

void
so::base::Surface::deleteMembers()
{
  if(mWindow not_eq nullptr)
  {
    glfwDestroyWindow(mWindow);

    mWindow = nullptr;
  }
}

