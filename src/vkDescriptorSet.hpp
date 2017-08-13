/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkDescriptorSet.hpp
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

#include <vulkan/vulkan.h>

namespace vk
{
  namespace eng
  {
    class
    DescriptorSet
    {
      public:
        DescriptorSet();

        DescriptorSet(VkDescriptorSetLayout descriptorSetLayout,
                      VkDescriptorPool      descriptorPool,
                      VkDevice              device,
                      VkBuffer              uniformBuffer,
                      VkImageView           textureImageView,
                      VkSampler             textureSampler);

        DescriptorSet(DescriptorSet const& other) = delete;

        DescriptorSet(DescriptorSet&& other) = delete;

        DescriptorSet& operator=(DescriptorSet const& other) = delete;

        DescriptorSet&
        operator=(DescriptorSet&& other) noexcept
        {
          mDescriptorSet = other.mDescriptorSet;

          other.mDescriptorSet = VK_NULL_HANDLE;

          return *this;
        }

        inline VkDescriptorSet& getVkDescriptorSet() { return mDescriptorSet; }

      private:
        VkDescriptorSet       mDescriptorSet;
    };
  } // namespace eng
} // namespace vk
