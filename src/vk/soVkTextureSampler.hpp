/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkTextureSampler.hpp
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

#include <vk/soVkLogicalDevice.hpp>
#include <vk/soVkTextureImage.hpp>
#include <vk/soVkImageViews.hpp>

#include <map>

namespace so
{
  namespace vk
  {
    class
    TextureSampler
    {
      using TextureImageMap =
        std::map<std::string, TextureImage>;

      public:
        TextureSampler();

        TextureSampler(SharedPtrLogicalDevice const& device,
                       SharedPtrCommandPool   const& commandPool);

        TextureSampler(TextureSampler const& other) = delete;

        TextureSampler(TextureSampler&& other) = delete;

        ~TextureSampler() noexcept;

        TextureSampler& operator=(TextureSampler const& other) = delete;

        TextureSampler&
        operator=(TextureSampler&& other) noexcept
        {
          destroyMembers();

          mSampler           = other.mSampler;
          mTextureImageViews = std::move(other.mTextureImageViews);
          mTextureImages     = std::move(other.mTextureImages);
          mDevice            = other.mDevice;
          mCommandPool       = other.mCommandPool;

          other.mSampler     = VK_NULL_HANDLE;
          other.mDevice      = SHARED_PTR_NULL_LOGICAL_DEVICE;
          other.mCommandPool = SHARED_PTR_NULL_COMMAND_POOL;
          return *this;
        }

        inline auto getVkSampler() { return mSampler; }

        inline auto& getTextureImageViews() { return mTextureImageViews; }

        inline auto& getTextureImages() { return mTextureImages; }

        bool
        addTexture(std::string const path, std::string const key = "");

      private:
        VkSampler              mSampler;

        ImageViews             mTextureImageViews;

        TextureImageMap        mTextureImages;

        SharedPtrLogicalDevice mDevice;

        SharedPtrCommandPool   mCommandPool;

        void
        destroyMembers();
    };
  } // namespace vk
} // namespace so