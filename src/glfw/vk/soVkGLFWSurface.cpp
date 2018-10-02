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

#include "soVkGLFWSurface.hpp"

so::vk::GLFWSurface::GLFWSurface()
  : base::Surface(), mSurface(VK_NULL_HANDLE), mInstance(VK_NULL_HANDLE) {}

so::vk::GLFWSurface::GLFWSurface(VkInstance const instance) 
  : base::Surface(), mSurface(VK_NULL_HANDLE), mInstance(instance) {}

so::vk::GLFWSurface::~GLFWSurface() noexcept { deleteMembers(); }

so::vk::GLFWSurface&
so::vk::GLFWSurface::operator=(GLFWSurface&& other) noexcept
{
  if(this is_eq &other)
  {
    return *this;
  }

  base::Surface::operator=(static_cast<base::Surface&&>(other));

  deleteMembers();

  mSurface  = other.mSurface;
  mInstance = other.mInstance;

  other.mSurface  = VK_NULL_HANDLE;
  other.mInstance = VK_NULL_HANDLE;

  return *this;
}

so::return_t
so::vk::GLFWSurface::getInstanceExtensions(char const*** extensions,
                                           size_type* count) const
{
  std::uint32_t ucount(0);

  *extensions = (glfwGetRequiredInstanceExtensions(&ucount));

  if(*extensions is_eq nullptr)
  {
    *count = 0;

    return failure;
  }

  *count = static_cast<size_type>(ucount);

  return success;
}

so::return_t
so::vk::GLFWSurface::createWindow(std::string   const& title,
                                  so::size_type const  width,
                                  so::size_type const  height)
{
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  auto const iWidth{ static_cast<int>(width) };
  auto const iHeight{ static_cast<int>(height) };

  mWindow = glfwCreateWindow(iWidth, iHeight, title.c_str(), nullptr, nullptr);

  return mWindow not_eq nullptr ? success : failure;
}

so::return_t
so::vk::GLFWSurface::createSurface(VkInstance instance)
{
  VkResult const result{ glfwCreateWindowSurface(instance,
                                                 mWindow,
                                                 nullptr,
                                                 &mSurface) };

  return result is_eq VK_SUCCESS ? success : failure;
}

void
so::vk::GLFWSurface::deleteMembers()
{
  if((mSurface not_eq VK_NULL_HANDLE) and (mInstance not_eq VK_NULL_HANDLE))
  {
    destroySurfaceKHR(mInstance, mSurface, nullptr);
  }
}

