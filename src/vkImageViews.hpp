/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      VkImageViews.hpp
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

#include <vkLogicalDevice.hpp>

namespace vk
{
  namespace eng
  {
    class
    ImageViews
    {
      public:
        ImageViews();

        ImageViews(SharedPtrLogicalDevice const& device,
                   std::vector<VkImage> const&   images,
                   VkFormat                      format,
                   VkImageAspectFlags            aspectFlags);

        ImageViews(ImageViews const& other) = delete;

        ImageViews(ImageViews&& other) = delete;

        ~ImageViews() noexcept;

        ImageViews&
        operator=(ImageViews const& other) = delete;

        ImageViews&
        operator=(ImageViews&& other) noexcept
        {
          destroyMembers();
          
          mImageViews = other.mImageViews;
          mDevice     = other.mDevice;

          other.mImageViews = std::vector<VkImageView>();
          other.mDevice     = SHARED_PTR_NULL_LOGICAL_DEVICE;

          return *this;
        }

        inline std::vector<VkImageView>& getVkImageViews()
        { return mImageViews; }

      private:
        std::vector<VkImageView> mImageViews;

        SharedPtrLogicalDevice   mDevice;

        void
        destroyMembers();
    };
  } // namespace eng
} // namespace vk
