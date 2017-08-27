/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkSurface.hpp
 *  @author    Bennet Carstensen
 *  @date      2017
 *  @copyright Copyright (c) 2017 Bennet Carstensen
 *
 *             This Source Code Form is subject to the terms of the
 *             Mozilla Public License, v. 2.0. If a copy of the MPL was
 *             not distributed with this file, You can obtain one at
 *             http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <vk/soVkInstance.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

const int WIDTH(800);
const int HEIGHT(600);

namespace so
{
  namespace vk
  {
    class
    Surface : public std::enable_shared_from_this<Surface>
    {
      public:
        Surface();

        Surface(GLFWwindow* window, SharedPtrInstance const& instance);

        Surface(Surface const& other) = delete;

        Surface(Surface&& other) = delete;

        ~Surface() noexcept;

        Surface&
        operator=(Surface const& other) = delete;

        Surface&
        operator=(Surface&& other) noexcept
        {
          mSurface  = other.mSurface;
          mWindow   = other.mWindow;
          mInstance = other.mInstance;

          other.mSurface  = VK_NULL_HANDLE;
          other.mWindow   = nullptr;
          other.mInstance = SHARED_PTR_NULL_INSTANCE;

          return *this;
        }

        inline VkSurfaceKHR getVkSurfaceKHR() { return mSurface; }

        inline GLFWwindow* getGLFWwindow() { return mWindow; }

      private:
        VkSurfaceKHR      mSurface;
        GLFWwindow*       mWindow;

        SharedPtrInstance mInstance;
    };

    using SharedPtrSurface = std::shared_ptr<Surface>;
  } // namespace vk
} // namespace so
