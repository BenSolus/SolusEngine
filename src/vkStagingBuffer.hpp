/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkStagingBuffer.hpp
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

namespace vk
{
  namespace eng
  {
    class
    StagingBuffer : public Buffer
    {
      public:
        StagingBuffer();

        StagingBuffer(SharedPtrLogicalDevice const& device,
                      VkDeviceSize                  size,
                      void const*                   res);

        StagingBuffer(StagingBuffer const& other) = delete;

        StagingBuffer(StagingBuffer&& other) = delete;

        StagingBuffer& operator=(StagingBuffer const& other) = delete;

        StagingBuffer&
        operator=(StagingBuffer&& other) noexcept
        {
          Buffer::operator=(static_cast<Buffer&&>(other));

          return *this;
        }
    };
  } // namespace eng
} // namespace vk
