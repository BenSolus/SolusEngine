/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vkImageViews.hpp>

#include <ccException.hpp>

vk::eng::ImageViews::ImageViews()
  : mImageViews(), mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE)
{}

vk::eng::ImageViews::ImageViews(SharedPtrLogicalDevice const& device,
                                std::vector<VkImage> const&   images,
                                VkFormat                      format,
                                VkImageAspectFlags            aspectFlags)
  : mImageViews(), mDevice(device)
{
  const std::size_t size(images.size());

  mImageViews.resize(size);

  for(size_t i(0); i < size; ++i)
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

    if(vkCreateImageView(device->getVkDevice(),
                         &createInfo,
                         nullptr,
                         &mImageViews[i]) != VK_SUCCESS)
    {
      cc::throw_with_nested<std::runtime_error>("failed to create image views!",
                                                PRETTY_FUNCTION_SIG);
    }
  }
}

vk::eng::ImageViews::~ImageViews() noexcept
{
  destroyMembers();
}

void
vk::eng::ImageViews::destroyMembers()
{
  VkDevice device(mDevice->getVkDevice());

  if(device != VK_NULL_HANDLE)
    for(size_t i(0); i < mImageViews.size(); ++i)
      vkDestroyImageView(device, mImageViews[i], nullptr);
}
