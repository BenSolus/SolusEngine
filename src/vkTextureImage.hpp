/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkTextureImage.hpp
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

#include <vkImage.hpp>

namespace vk
{
  namespace eng
  {
    class
    TextureImage : public Image
    {
      public:
        TextureImage();

        TextureImage(SharedPtrLogicalDevice const& device,
                     std::string const&            path,
                     VkCommandPool                 commandPool);

        TextureImage(TextureImage const& other) = delete;

        TextureImage(TextureImage&& other) = delete;

        TextureImage& operator=(TextureImage const& other) = delete;

        TextureImage&
        operator=(TextureImage&& other) noexcept
        {
          Image::operator=(static_cast<Image&&>(other));

          return *this;
        }

      private:
        void
        copyBufferToImage(VkCommandPool commandPool,
                          VkBuffer      buffer,
                          uint32_t      width,
                          uint32_t      height);
    };
  } // namespace eng
} // namespace vk
