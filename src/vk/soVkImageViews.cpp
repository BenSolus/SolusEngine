/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vk/soVkImageViews.hpp>

#include <utils/err/soException.hpp>

so::vk::ImageViews::ImageViews()
  : mImageViews(), mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE)
{}

so::vk::ImageViews::ImageViews(SharedPtrLogicalDevice const& device)
  : mImageViews(), mDevice(device)
{}

so::vk::ImageViews::ImageViews(SharedPtrLogicalDevice const& device,
                               std::vector<VkImage> const&   images,
                               VkFormat                      format,
                               VkImageAspectFlags            aspectFlags)
  : mImageViews(), mDevice(device)
{
  std::size_t const size(images.size());

  mImageViews.resize(size);

  for(std::size_t i(0); i < size; ++i)
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
      throw utils::err::Exception<std::runtime_error>("failed to create image "
                                                      "views!",
                                                      PRETTY_FUNCTION_SIG);
  }
}

so::vk::ImageViews::~ImageViews() noexcept
{
  destroyMembers();
}

void
so::vk::ImageViews::addImageViews(std::vector<VkImage> const&   images,
                                  VkFormat                      format,
                                  VkImageAspectFlags            aspectFlags)
{
  std::size_t const oldSize(mImageViews.size());
  std::size_t const newSize(mImageViews.size() + images.size());

  mImageViews.resize(newSize);

  for(std::size_t i(oldSize); i < newSize; ++i)
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
      throw utils::err::Exception<std::runtime_error>("failed to create image "
                                                      "view!",
                                                      PRETTY_FUNCTION_SIG);
  }
}

void
so::vk::ImageViews::destroyMembers()
{
  VkDevice device(mDevice->getVkDevice());

  if(device not_eq VK_NULL_HANDLE)
    for(size_t i(0); i < mImageViews.size(); ++i)
      vkDestroyImageView(device, mImageViews[i], nullptr);
}
