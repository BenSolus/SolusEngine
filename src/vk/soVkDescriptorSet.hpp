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

#include <vk/soVkTextureSampler.hpp>
#include <vk/soVkUniformBuffer.hpp>

namespace so
{
  namespace vk
  {
    class
    DescriptorSet
    {
      public:
        DescriptorSet();

        DescriptorSet(VkDescriptorSetLayout descriptorSetLayout,
                      VkDescriptorPool      descriptorPool,
                      VkDevice              device,
                      std::string const     textureKey,
                      TextureSampler&       textureSampler,
                      UniformBuffer&        uniformBuffer);

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
  } // namespace vk
} // namespace so
