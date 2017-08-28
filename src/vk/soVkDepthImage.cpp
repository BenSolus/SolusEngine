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