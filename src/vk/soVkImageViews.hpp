/*
 * Copyright (C) 2017-2018 by Bennet Carstensen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

/**
 *  @file      soVkImageViews.hpp
 *  @author    Bennet Carstensen
 *  @date      2017
 *  @copyright Copyright (c) 2017-2018 Bennet Carstensen
 *
 *             Permission is hereby granted, free of charge, to any person
 *             obtaining a copy of this software and associated documentation
 *             files (the "Software"), to deal in the Software without
 *             restriction, including without limitation the rights to use,
 *             copy, modify, merge, publish, distribute, sublicense, and/or
 *             sell copies of the Software, and to permit persons to whom the
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

#include "soVkLogicalDevice.hpp"

namespace so {
namespace vk {
    
class
ImageViews
{
  public:
    ImageViews();

    ImageViews(ImageViews const& other) = delete;

    ImageViews(ImageViews&& other) = delete;

    ~ImageViews() noexcept;

    ImageViews&
    operator=(ImageViews const& other) = delete;

    ImageViews&
    operator=(ImageViews&& other) noexcept;

    return_t
    initialize(SharedPtrLogicalDevice const& device,
               std::vector<VkImage>   const& images,
               VkFormat                      format,
               VkImageAspectFlags            aspectFlags);

    return_t
    reset(std::vector<VkImage> const& images,
          VkFormat                    format,
          VkImageAspectFlags          aspectFlags);

    inline std::vector<VkImageView> const&
    getVkImageViewsRef() const { return mImageViews; }

    return_t
    addImageViews(std::vector<VkImage> const&   images,
                  VkFormat                      format,
                  VkImageAspectFlags            aspectFlags);

  private:
    std::vector<VkImageView> mImageViews;

    SharedPtrLogicalDevice   mDevice;

    return_t
    initializeMembers(std::vector<VkImage> const& images,
                      VkFormat                    format,
                      VkImageAspectFlags          aspectFlags);

    void
    destroyMembers();
};

} // namespace vk
} // namespace so

