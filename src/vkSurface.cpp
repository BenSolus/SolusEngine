/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vkSurface.hpp>

#include <ccException.hpp>

vk::eng::Surface::Surface()
  : mSurface(VK_NULL_HANDLE),
    mWindow(nullptr),
    mInstance(SHARED_PTR_NULL_INSTANCE)
{
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  mWindow = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

vk::eng::Surface::Surface(GLFWwindow* window, SharedPtrInstance const& instance)
  : mSurface(VK_NULL_HANDLE),
    mWindow(window),
    mInstance(instance)
{
  if(glfwCreateWindowSurface(instance->getVkInstance(), window, nullptr, &mSurface) != VK_SUCCESS)
  {
    cc::throw_with_nested<std::runtime_error>("failed to create window "
                                              "surface!",
                                              PRETTY_FUNCTION_SIG);
  }
}

vk::eng::Surface::~Surface() noexcept
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
