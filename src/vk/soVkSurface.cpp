/*
 * Copyright (C) 2017 by Bennet Carstensen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <vk/soVkSurface.hpp>

#include <utils/err/soException.hpp>

so::vk::Surface::Surface()
  : mSurface(VK_NULL_HANDLE),
    mWindow(nullptr),
    mInstance(SHARED_PTR_NULL_INSTANCE)
{
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  mWindow = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

so::vk::Surface::Surface(GLFWwindow* window, SharedPtrInstance const& instance)
  : mSurface(VK_NULL_HANDLE),
    mWindow(window),
    mInstance(instance)
{
  VkResult const result(glfwCreateWindowSurface(instance->getVkInstance(),
                                                window,
                                                nullptr,
                                                &mSurface));

  if(result not_eq VK_SUCCESS)
    throw utils::err::Exception<std::runtime_error>("failed to create window "
                                                    "surface!",
                                                    PRETTY_FUNCTION_SIG);
}

so::vk::Surface::~Surface() noexcept
{
  VkInstance instance = mInstance->getVkInstance();

  if((mSurface != VK_NULL_HANDLE) && (instance != VK_NULL_HANDLE))
    vkDestroySurfaceKHR(instance, mSurface, nullptr);

  if(mWindow != nullptr)
  {
    glfwDestroyWindow(mWindow);

    glfwTerminate();
  }
}
