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
 *  @file      soVkPhysicalDevice.hpp
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
#include <vk/soVkSurface.hpp>

extern const std::vector<const char*> DEVICE_EXTENSIONS;

namespace so
{
  namespace vk
  {
    class
    PhysicalDevice
    {
      public:
        PhysicalDevice();

        PhysicalDevice
        (VkPhysicalDevice device,
         SharedPtrInstance const& instance = SHARED_PTR_NULL_INSTANCE);

        PhysicalDevice(SharedPtrInstance const& instance, VkSurfaceKHR surface);

        PhysicalDevice(PhysicalDevice const& other) = delete;

        PhysicalDevice(PhysicalDevice&& other) = delete;

        PhysicalDevice& operator=(PhysicalDevice const& other) = delete;

        PhysicalDevice&
        operator=(PhysicalDevice&& other) noexcept
        {
          mPhysicalDevice = other.mPhysicalDevice;
          mInstance       = other.mInstance;

          other.mPhysicalDevice = VK_NULL_HANDLE;
          other.mInstance       = SHARED_PTR_NULL_INSTANCE;

          return *this;
        };

        inline VkPhysicalDevice getVkPhysicalDevice()
        { return mPhysicalDevice; }

        inline SharedPtrInstance getInstance()
        { return mInstance->shared_from_this(); }

      protected:
        VkPhysicalDevice  mPhysicalDevice;
        SharedPtrInstance mInstance;

        bool
        checkDeviceExtensionSupport(VkPhysicalDevice device);

        bool
        isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
    };
  } // namespace vk
} //namespace so
