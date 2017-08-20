/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkIndexBuffer.hpp
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
#include <vkCommandPool.hpp>

namespace vk
{
  namespace eng
  {
    class
    IndexBuffer : public Buffer
    {
      public:
        IndexBuffer();

        IndexBuffer(SharedPtrLogicalDevice const& device,
                    SharedPtrCommandPool const&   commandPool);

        IndexBuffer(IndexBuffer const& other) = delete;

        IndexBuffer(IndexBuffer&& other) = delete;

        IndexBuffer& operator=(IndexBuffer const& other) = delete;

        IndexBuffer&
        operator=(IndexBuffer&& other) noexcept
        {
          Buffer::operator=(static_cast<Buffer&&>(other));

          return *this;
        }
    };
  } // namespace eng
} // namespace vk
