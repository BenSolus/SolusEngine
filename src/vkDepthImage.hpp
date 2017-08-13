/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkDepthImage.hpp
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
    DepthImage : public Image
    {
      public:
        DepthImage();

        DepthImage(SharedPtrLogicalDevice const& device,
                   VkExtent2D                    extend,
                   VkCommandPool                 commandPool);

        DepthImage(DepthImage const& other) = delete;

        DepthImage(DepthImage&& other) = delete;

        DepthImage& operator=(DepthImage const& other) = delete;

        DepthImage&
        operator=(DepthImage&& other) noexcept
        {
          Image::operator=(static_cast<Image&&>(other));

          mFormat = other.mFormat;

          other.mFormat = VK_FORMAT_UNDEFINED;

          return *this;
        }

        inline VkFormat getVkFormat() { return mFormat; }

      private:
        VkFormat mFormat;

        VkFormat
        findSupportedFormat(const std::vector<VkFormat>& candidates,
                            VkImageTiling                tiling,
                            VkFormatFeatureFlags         features);

        VkFormat
        findDepthFormat();
    };
  } // namespace eng
} // namespace vk