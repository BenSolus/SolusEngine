/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkVertexBuffer.hpp
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

#include <vk/soVkBuffer.hpp>
#include <vk/soVkCommandPool.hpp>

namespace so
{
  namespace vk
  {
    class
    VertexBuffer : public Buffer
    {
      public:
        VertexBuffer();

        VertexBuffer(SharedPtrLogicalDevice const& device,
                     SharedPtrCommandPool const&   commandPool);

        VertexBuffer(VertexBuffer const& other) = delete;

        VertexBuffer(VertexBuffer&& other) = delete;

        VertexBuffer& operator=(VertexBuffer const& other) = delete;

        VertexBuffer&
        operator=(VertexBuffer&& other) noexcept
        {
          Buffer::operator=(static_cast<Buffer&&>(other));

          return *this;
        }
    };
  } // namespace vk
} // namespace so
