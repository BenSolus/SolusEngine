/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vk/soVkDepthImage.hpp>

so::vk::DepthImage::DepthImage() : Image(), mFormat(VK_FORMAT_UNDEFINED) {}

so::vk::DepthImage::DepthImage(SharedPtrLogicalDevice const& device,
                               SharedPtrCommandPool const&   commandPool,
                               SwapChain&                    swapChain)
  : Image(device), mFormat(VK_FORMAT_UNDEFINED)
{
  try
  {
    mFormat = findDepthFormat();

    createImage(swapChain.getVkExtent().width,
                swapChain.getVkExtent().height,
                mFormat,
                VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    transitionImageLayout(commandPool->getVkCommandPool(),
                          mFormat,
                          VK_IMAGE_LAYOUT_UNDEFINED,
                          VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
  }
  catch(...)
  {
    std::throw_with_nested(utils::err::Exception<std::runtime_error>
      ("Caught exception:", PRETTY_FUNCTION_SIG));
  }
}

VkFormat
so::vk::DepthImage::findSupportedFormat
  (const std::vector<VkFormat>& candidates,
   VkImageTiling                tiling,
   VkFormatFeatureFlags         features)
{
  for(VkFormat format : candidates)
  {
    VkFormatProperties props;

    vkGetPhysicalDeviceFormatProperties(mDevice->getVkPhysicalDevice(),
                                        format,
                                        &props);

    if((tiling == VK_IMAGE_TILING_LINEAR) && ((props.linearTilingFeatures & features) == features))
    {
      return format;
    }
    else if((tiling == VK_IMAGE_TILING_OPTIMAL) && ((props.optimalTilingFeatures & features) == features))
    {
      return format;
    }
  }

  throw utils::err::Exception<std::runtime_error>("failed to find supported "
                                                  "format!",
                                                  PRETTY_FUNCTION_SIG);

  return VK_FORMAT_UNDEFINED;
}

VkFormat
so::vk::DepthImage::findDepthFormat()
{
  try {
    return findSupportedFormat
             ({ VK_FORMAT_D32_SFLOAT,
                VK_FORMAT_D32_SFLOAT_S8_UINT,
                VK_FORMAT_D24_UNORM_S8_UINT },
              VK_IMAGE_TILING_OPTIMAL,
              VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
  }
  catch(...)
  {
    std::throw_with_nested(utils::err::Exception<std::runtime_error>
      ("Caught exception:", PRETTY_FUNCTION_SIG));
  }

  // return VK_FORMAT_UNDEFINED;
}