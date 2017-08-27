/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkLogicalDevice.hpp
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

#include <vk/soVkPhysicalDevice.hpp>
#include <vk/soVkSurface.hpp>

namespace so
{
  namespace vk
  {
    class
    LogicalDevice : public PhysicalDevice,
                    public std::enable_shared_from_this<LogicalDevice>
    {
      public:
        LogicalDevice();

        LogicalDevice(SharedPtrInstance const& device, VkSurfaceKHR surface);

        LogicalDevice(LogicalDevice const& other) = delete;

        LogicalDevice(LogicalDevice&& other) = delete;

        ~LogicalDevice() noexcept;

        LogicalDevice& operator=(LogicalDevice const& other) = delete;

        LogicalDevice&
        operator=(LogicalDevice&& other) noexcept
        {
          PhysicalDevice::operator=(static_cast<PhysicalDevice&&>(other));

          destroyMembers();

          mDevice        = other.mDevice;
          mGraphicsQueue = other.mGraphicsQueue;
          mPresentQueue  = other.mPresentQueue;

          other.mDevice        = VK_NULL_HANDLE;
          other.mGraphicsQueue = VK_NULL_HANDLE;
          other.mPresentQueue  = VK_NULL_HANDLE;

          return *this;
        }

        inline VkDevice getVkDevice() { return mDevice; }

        inline VkQueue getGraphicsVkQueue() { return mGraphicsQueue; }

        inline VkQueue getPresentVkQueue() { return mPresentQueue; }

      private:
        VkDevice                mDevice;
        VkQueue                 mGraphicsQueue;
        VkQueue                 mPresentQueue;

        void
        destroyMembers();
    };

    using SharedPtrLogicalDevice = std::shared_ptr<so::vk::LogicalDevice>;

  } // namespace vk
} //namespace so

extern so::vk::SharedPtrLogicalDevice const SHARED_PTR_NULL_LOGICAL_DEVICE;
