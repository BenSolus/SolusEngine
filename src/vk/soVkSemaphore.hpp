/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkSemaphore.hpp
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

#include <vk/soVkLogicalDevice.hpp>

namespace so
{
  namespace vk
  {
    class
    Semaphore
    {
      public:
        Semaphore();

        Semaphore(SharedPtrLogicalDevice const& device);

        Semaphore(Semaphore const& other) = delete;

        Semaphore(Semaphore&& other) = delete;

        ~Semaphore() noexcept;

        Semaphore&
        operator=(Semaphore const& other) = delete;

        Semaphore&
        operator=(Semaphore&& other) noexcept
        {
          destroyMembers();

          mSemaphore = other.mSemaphore;
          mDevice    = other.mDevice;

          other.mSemaphore = VK_NULL_HANDLE;
          other.mDevice    = SHARED_PTR_NULL_LOGICAL_DEVICE;

          return *this;
        }

        inline VkSemaphore getVkSemaphore() { return mSemaphore; }
        
      private:
        VkSemaphore            mSemaphore;

        SharedPtrLogicalDevice mDevice;

        void
        destroyMembers();
    };
  } // namespace vk
} // namespace so
