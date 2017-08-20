/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkBuffer.hpp
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

#include <vkResource.hpp>
#include <vkLogicalDevice.hpp>

namespace vk
{
  namespace eng
  {
    class
    Buffer : public Resource<VkBuffer>
    {
      public:
        Buffer();

        Buffer(SharedPtrLogicalDevice const& device,
               VkDeviceSize                  size,
               VkBufferUsageFlags            usage,
               VkMemoryPropertyFlags         properties);

        Buffer(Buffer const& other) = delete;

        Buffer(Buffer&& other) = delete;

        virtual ~Buffer() noexcept;

        Buffer& operator=(Buffer const& other) = delete;

        Buffer&
        operator=(Buffer&& other) noexcept
        {
          destroyMembers();

          Resource::operator=(static_cast<Resource&&>(other));

          return *this;
        }

      protected:
        void
        copyBuffer(VkBuffer      srcBuffer,
                   VkDeviceSize  size,
                   VkCommandPool commandPool);

      private:
        void
        destroyMembers();
    };
  } // namespace eng
} // namespace vk
