/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkImage.hpp
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

#include <vk/soVkResource.hpp>
#include <vk/soVkLogicalDevice.hpp>

namespace so
{
  namespace vk
  {
    class
    Image : public Resource<VkImage>
    {
      public:
        Image();

        Image(SharedPtrLogicalDevice const& device);

        Image(Image const& other) = delete;

        Image(Image&& other);

        ~Image() noexcept;

        Image& operator=(Image const& other) = delete;

        Image&
        operator=(Image&& other) noexcept
        {
          destroyMembers();

          Resource::operator=(static_cast<Resource&&>(other));

          return *this;
        }

      protected:
        void
        createImage(uint32_t              width,
                    uint32_t              height,
                    VkFormat              format,
                    VkImageTiling         tiling,
                    VkImageUsageFlags     usage,
                    VkMemoryPropertyFlags properties);

        void
        transitionImageLayout(VkCommandPool commandPool,
                              VkFormat      format,
                              VkImageLayout oldLayout,
                              VkImageLayout newLayout);

        bool
        hasStencilComponent(VkFormat format);

      private:
        void
        destroyMembers();
    };
  } // namespace vk
} // namespace so
