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
 *  @file      soVkImageViews.hpp
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

namespace so
{
  namespace vk
  {
    class
    ImageViews
    {
      public:
        ImageViews();

        ImageViews(SharedPtrLogicalDevice const& device);

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

        inline auto& getVkImageViews() { return mImageViews; }

        void
        addImageViews(std::vector<VkImage> const&   images,
                      VkFormat                      format,
                      VkImageAspectFlags            aspectFlags);
      private:
        std::vector<VkImageView> mImageViews;

        SharedPtrLogicalDevice   mDevice;

        void
        destroyMembers();
    };
  } // namespace vk
} // namespace so
