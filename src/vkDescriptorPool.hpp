/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkDescriptorPool.hpp
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

#include <vkLogicalDevice.hpp>

namespace vk
{
  namespace eng
  {
    class
    DescriptorPool
    {
      public:
        DescriptorPool();

        DescriptorPool(SharedPtrLogicalDevice const& device);

        DescriptorPool(DescriptorPool const& other) = delete;

        DescriptorPool(DescriptorPool&& other) = delete;

        ~DescriptorPool() noexcept;

        DescriptorPool& operator=(DescriptorPool const& other) = delete;

        DescriptorPool&
        operator=(DescriptorPool&& other) noexcept
        {
          destroyMembers();

          mDescriptorPool = other.mDescriptorPool;
          mDevice         = other.mDevice;

          other.mDescriptorPool = VK_NULL_HANDLE;
          other.mDevice         = SHARED_PTR_NULL_LOGICAL_DEVICE;

          return *this;
        }

        inline VkDescriptorPool getVkDescriptorPool()
        { return mDescriptorPool; }
        
      private:
        VkDescriptorPool       mDescriptorPool;

        SharedPtrLogicalDevice mDevice;
        void
        destroyMembers();
    };
  } // namespace eng
} // namespace vk
