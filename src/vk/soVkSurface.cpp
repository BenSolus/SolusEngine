/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
