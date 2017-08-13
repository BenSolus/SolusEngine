/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkUniformBuffer.hpp
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

#include <vkBuffer.hpp>

#include <glm/glm.hpp>

namespace vk
{
  namespace eng
  {
    struct UniformBufferObject
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    class
    UniformBuffer : public Buffer
    {
      public:
        UniformBuffer();

        UniformBuffer(SharedPtrLogicalDevice const& device);

        UniformBuffer(UniformBuffer const& other) = delete;

        UniformBuffer(UniformBuffer&& other) = delete;

        UniformBuffer& operator=(UniformBuffer const& other) = delete;

        UniformBuffer&
        operator=(UniformBuffer&& other) noexcept
        {
          Buffer::operator=(static_cast<Buffer&&>(other));

          return *this;
        }

        void
        update(VkExtent2D swapChainExtent);
    };
  } // namespace eng
} // namespace vk
