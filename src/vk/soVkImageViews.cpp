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

#include "soVkImageViews.hpp"

#include "soDefinitions.hpp"

so::vk::ImageViews::ImageViews()
  : mImageViews(), mDevice(LogicalDevice::getSharedPtrNullDevice())
{}

so::vk::ImageViews::~ImageViews() noexcept
{
  destroyMembers();
}

so::vk::ImageViews&
so::vk::ImageViews::operator=(ImageViews&& other) noexcept
{
  if(this is_eq &other)
  {
    return *this;
  }

  destroyMembers();
          
  mImageViews = other.mImageViews;
  mDevice     = other.mDevice;

  other.mImageViews = std::vector<VkImageView>();
  other.mDevice     = LogicalDevice::getSharedPtrNullDevice();

  return *this;
}

so::return_t
so::vk::ImageViews::initialize(SharedPtrLogicalDevice const& device,
                               std::vector<VkImage>   const& images,
                               VkFormat                      format,
                               VkImageAspectFlags            aspectFlags)
{
  mDevice = device;

  std::size_t const size(images.size());

  mImageViews.resize(size);

  for(uindex i(0); i < size; ++i)
  {
    VkImageViewCreateInfo createInfo = {};

    createInfo.sType                           =
      VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image                           = images[i];
    createInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format                          = format;
    createInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask     = aspectFlags;
    createInfo.subresourceRange.baseMipLevel   = 0;
    createInfo.subresourceRange.levelCount     = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount     = 1;

    VkResult const result(vkCreateImageView(device->getVkDevice(),
                                            &createInfo,
                                            nullptr,
                                            &mImageViews[i]));

    if(result not_eq VK_SUCCESS)
    {
      return failure;
    }
  }

  return success;
}

so::return_t
so::vk::ImageViews::addImageViews(std::vector<VkImage> const& images,
                                  VkFormat                    format,
                                  VkImageAspectFlags          aspectFlags)
{
  std::size_t const oldSize(mImageViews.size());
  std::size_t const newSize(mImageViews.size() + images.size());

  mImageViews.resize(newSize);

  for(uindex i(oldSize); i < newSize; ++i)
  {
    VkImageViewCreateInfo createInfo({});

    createInfo.sType                           =
      VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image                           = images[i - oldSize];
    createInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format                          = format;
    createInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask     = aspectFlags;
    createInfo.subresourceRange.baseMipLevel   = 0;
    createInfo.subresourceRange.levelCount     = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount     = 1;

    VkResult const result(vkCreateImageView(mDevice->getVkDevice(),
                                            &createInfo,
                                            nullptr,
                                            &mImageViews[i]));

    if(result not_eq VK_SUCCESS)
    {
      return failure;
    }
  }

  return success;
}

void
so::vk::ImageViews::destroyMembers()
{
  VkDevice device(mDevice->getVkDevice());

  if(device not_eq VK_NULL_HANDLE)
  { 
    for(auto imageView : mImageViews)
    {
      vkDestroyImageView(device, imageView, nullptr);
    }  
  }
}

