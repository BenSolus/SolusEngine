/*
 * Copyright (C) 2017 by Bennet Carstensen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 *  @file      soVkTextureSampler.hpp
 *  @author    Bennet Carstensen
 *  @date      2017
 *  @copyright Copyright (c) 2017 Bennet Carstensen
 *
 *             Permission is hereby granted, free of charge, to any person
 *             obtaining a copy of this software and associated documentation
 *             files (the "Software"), to deal in the Software without
 *             restriction, including without limitation the rights to use,
 *             copy, modify, merge, publish, distribute, sublicense, and/or sell
 *             copies of the Software, and to permit persons to whom the
 *             Software is furnished to do so, subject to the following
 *             conditions:
 *
 *             The above copyright notice and this permission notice shall be
 *             included in all copies or substantial portions of the Software.
 *
 *             THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *             EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *             OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *             NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *             HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *             WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *             FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *             OTHER DEALINGS IN THE SOFTWARE.
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

        auto getVkSampler() { return mSampler; }

        auto& getTextureImageViews() { return mTextureImageViews; }

        auto& getTextureImages() { return mTextureImages; }

        std::string
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