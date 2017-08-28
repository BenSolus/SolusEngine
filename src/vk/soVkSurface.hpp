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

/**
 *  @file      soVkSurface.hpp
 *  @author    Bennet Carstensen
 *  @date      2017
 *  @copyright Copyright (c) 2017 Bennet Carstensen
 *
 *             Permission is hereby granted, free of charge, to any person
 *             obtaining a copy of this software and associated documentation
 *             files (the "Software"), to deal in the Software without
 *             restriction, including without limitation the rights to use,
 *             copy, modify, merge, publish, distribute, sublicense, and/or sell
 *             copies of the Software, and to permit persons to whom the
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
