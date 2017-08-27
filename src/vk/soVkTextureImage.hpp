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

#include <vk/soVkCommandPool.hpp>
#include <vk/soVkImage.hpp>

namespace so
{
  namespace vk
  {
    class
    TextureImage : public Image
    {
      public:
        TextureImage();

        TextureImage(SharedPtrLogicalDevice const& device,
                     SharedPtrCommandPool  const&  commandPool,
                     std::string const             path,
                     std::size_t const             imageViewIdx);

        TextureImage(TextureImage const& other) = delete;

        TextureImage(TextureImage&& other) noexcept;

        TextureImage& operator=(TextureImage const& other) = delete;

        TextureImage&
        operator=(TextureImage&& other) noexcept
        {
          Image::operator=(static_cast<Image&&>(other));

          return *this;
        }

        inline auto getImageViewIdx() { return mImageViewIdx; }

      private:
        std::size_t mImageViewIdx;

        void
        copyBufferToImage(VkCommandPool commandPool,
                          VkBuffer      buffer,
                          uint32_t      width,
                          uint32_t      height);
    };
  } // namespace vk
} // namespace so
