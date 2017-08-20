/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vkTextureImage.hpp>

#include <vkStagingBuffer.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

vk::eng::TextureImage::TextureImage() : Image() {}

vk::eng::TextureImage::TextureImage(SharedPtrLogicalDevice const& device,
                                    SharedPtrCommandPool  const&  commandPool,
                                    std::string const&            path)
  : Image(device)
{
  int texWidth, texHeight, texChannels;

  stbi_uc* pixels = stbi_load((path).c_str(),
                              &texWidth,
                              &texHeight,
                              &texChannels,
                              STBI_rgb_alpha);

  VkDeviceSize imageSize = static_cast<VkDeviceSize>(texWidth * texHeight * 4);

  if(!pixels)
    cc::throw_with_nested<std::runtime_error>("failed to load texture image!",
                                              PRETTY_FUNCTION_SIG);

  StagingBuffer stagingBuffer;

  try
  {
    stagingBuffer = StagingBuffer(mDevice, imageSize, pixels);
  }
  catch(...)
  {
    cc::throw_with_nested<std::runtime_error>("Caught exception:",
                                              PRETTY_FUNCTION_SIG);
  }

  stbi_image_free(pixels);

  try
  {
    createImage(static_cast<uint32_t>(texWidth),
                static_cast<uint32_t>(texHeight),
                VK_FORMAT_R8G8B8A8_UNORM,
                VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);


    transitionImageLayout(commandPool->getVkCommandPool(),
                          VK_FORMAT_R8G8B8A8_UNORM,
                          VK_IMAGE_LAYOUT_PREINITIALIZED,
                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    copyBufferToImage(commandPool->getVkCommandPool(),
                      stagingBuffer.getResource(),
                      static_cast<uint32_t>(texWidth),
                      static_cast<uint32_t>(texHeight));

    transitionImageLayout(commandPool->getVkCommandPool(),
                          VK_FORMAT_R8G8B8A8_UNORM,
                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                          VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
  }
  catch(...)
  {
    cc::throw_with_nested<std::runtime_error>("Caught exception:",
                                              PRETTY_FUNCTION_SIG);
  }
}

void
vk::eng::TextureImage::copyBufferToImage(VkCommandPool commandPool,
                                         VkBuffer      buffer,
                                         uint32_t      width,
                                         uint32_t      height)
{
  VkCommandBuffer commandBuffer(beginSingleTimeCommands(commandPool));

  VkBufferImageCopy region = {};

  region.bufferOffset                    = 0;
  region.bufferRowLength                 = 0;
  region.bufferImageHeight               = 0;
  region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
  region.imageSubresource.mipLevel       = 0;
  region.imageSubresource.baseArrayLayer = 0;
  region.imageSubresource.layerCount     = 1;
  region.imageOffset                     = { 0, 0, 0 };
  region.imageExtent                     = { width, height, 1 };

  vkCmdCopyBufferToImage(commandBuffer,
                         buffer,
                         mResource,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                         1,
                         &region);

  endSingleTimeCommands(commandPool, commandBuffer);
}