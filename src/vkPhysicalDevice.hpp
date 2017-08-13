/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkPhysicalDevice.hpp
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

#include <vkInstance.hpp>
#include <vkSurface.hpp>

extern const std::vector<const char*> DEVICE_EXTENSIONS;

namespace vk
{
  namespace eng
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
  } // namespace eng
} //namespace vk
