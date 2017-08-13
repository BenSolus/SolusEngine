/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkDescriptorSetLayout.hpp
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
    DescriptorSetLayout
    {
      public:
        DescriptorSetLayout();

        DescriptorSetLayout(SharedPtrLogicalDevice const& device);

        DescriptorSetLayout(DescriptorSetLayout const& other) = delete;

        DescriptorSetLayout(DescriptorSetLayout&& other) = delete;

        ~DescriptorSetLayout() noexcept;

        DescriptorSetLayout&
        operator=(DescriptorSetLayout const& other) = delete;

        DescriptorSetLayout&
        operator=(DescriptorSetLayout&& other) noexcept
        {
          destroyMembers();

          mDescriptorSetLayout = other.mDescriptorSetLayout;
          mDevice              = other.mDevice;

          other.mDescriptorSetLayout = VK_NULL_HANDLE;
          other.mDevice              = SHARED_PTR_NULL_LOGICAL_DEVICE;

          return *this;
        }

        inline VkDescriptorSetLayout& getVkDescriptorSetLayout()
        { return mDescriptorSetLayout; }

      private:
        VkDescriptorSetLayout  mDescriptorSetLayout;

        SharedPtrLogicalDevice mDevice;

        void
        destroyMembers();
    };
  } // namespace eng
} // namespace vk
