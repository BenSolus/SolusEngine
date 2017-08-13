/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkTextureSampler.hpp
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
    TextureSampler
    {
      public:
        TextureSampler();

        TextureSampler(SharedPtrLogicalDevice const& device);

        TextureSampler(TextureSampler const& other) = delete;

        TextureSampler(TextureSampler&& other) = delete;

        ~TextureSampler() noexcept;

        TextureSampler& operator=(TextureSampler const& other) = delete;

        TextureSampler&
        operator=(TextureSampler&& other) noexcept
        {
          destroyMembers();

          mSampler = other.mSampler;
          mDevice  = other.mDevice;

          other.mSampler = VK_NULL_HANDLE;
          other.mDevice  = SHARED_PTR_NULL_LOGICAL_DEVICE;

          return *this;
        }

        inline VkSampler getVkSampler() { return mSampler; }

      private:
        VkSampler              mSampler;

        SharedPtrLogicalDevice mDevice;

        void
        destroyMembers();
    };
  } // namespace eng
} // namespace vk