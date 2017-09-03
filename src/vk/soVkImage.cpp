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

#include <vk/soVkImage.hpp>

so::vk::Image::Image() : Resource() {}

so::vk::Image::Image(SharedPtrLogicalDevice const& device)
  : Resource(device)
{}

so::vk::Image::Image(Image&& other) noexcept
  : Resource(static_cast<Resource&&>(other))
{}

so::vk::Image::~Image() noexcept { destroyMembers(); }

void
so::vk::Image::createImage(uint32_t              width,
                           uint32_t              height,
                           VkFormat              format,
                           VkImageTiling         tiling,
                           VkImageUsageFlags     usage,
                           VkMemoryPropertyFlags properties)
{
  VkImageCreateInfo imageInfo = {};

  imageInfo.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType     = VK_IMAGE_TYPE_2D;
  imageInfo.format        = format;
  imageInfo.extent.width  = width;
  imageInfo.extent.height = height;
  imageInfo.extent.depth  = 1;
  imageInfo.mipLevels     = 1;
  imageInfo.arrayLayers   = 1;
  imageInfo.samples       = VK_SAMPLE_COUNT_1_BIT;
  imageInfo.tiling        = tiling;
  imageInfo.usage         = usage;
  imageInfo.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;

  VkDevice vkDevice(mDevice->getVkDevice());

  VkResult result(vkCreateImage(vkDevice, &imageInfo, nullptr, &mResource));

  if(result not_eq VK_SUCCESS)
    throw utils::err::Exception<std::runtime_error>("failed to create image!",
                                                    PRETTY_FUNCTION_SIG);

  VkMemoryRequirements memRequirements;

  vkGetImageMemoryRequirements(vkDevice, mResource, &memRequirements);

  VkMemoryAllocateInfo allocInfo = {};

  allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize  = memRequirements.size;
  allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits,
                                             properties);

  result = vkAllocateMemory(vkDevice, &allocInfo, nullptr, &mDeviceMemory);

  if(result not_eq VK_SUCCESS)
    throw utils::err::Exception<std::runtime_error>("failed to allocate image "
                                                    "memory!",
                                                    PRETTY_FUNCTION_SIG);

  vkBindImageMemory(vkDevice, mResource, mDeviceMemory, 0);
}

void
so::vk::Image::transitionImageLayout(VkCommandPool commandPool,
                                     VkFormat      format,
                                     VkImageLayout oldLayout,
                                     VkImageLayout newLayout)
{
  VkCommandBuffer commandBuffer(beginSingleTimeCommands(commandPool));

  VkImageMemoryBarrier barrier = {};

  barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.oldLayout                       = oldLayout;
  barrier.newLayout                       = newLayout;
  barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
  barrier.image                           = mResource;

  if(newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
  {
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

    if(hasStencilComponent(format))
      barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

  } else
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

  barrier.subresourceRange.baseMipLevel   = 0;
  barrier.subresourceRange.levelCount     = 1;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount     = 1;

  if((oldLayout == VK_IMAGE_LAYOUT_PREINITIALIZED) && (newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL))
  {
    barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  }
  else if((oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) && (newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL))
  {
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  }
  else if((oldLayout == VK_IMAGE_LAYOUT_UNDEFINED) && (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL))
  {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
                            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  } else
    throw utils::err::Exception<std::invalid_argument>("unsupported layout "
                                                       "transition!",
                                                       PRETTY_FUNCTION_SIG);

  vkCmdPipelineBarrier(commandBuffer,
                       VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                       VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                       0,
                       0, nullptr,
                       0, nullptr,
                       1, &barrier);

  endSingleTimeCommands(commandPool, commandBuffer);
}

bool
so::vk::Image::hasStencilComponent(VkFormat format)
{
  return format == VK_FORMAT_D32_SFLOAT_S8_UINT ||
         format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void
so::vk::Image::destroyMembers()
{
  VkDevice device(mDevice->getVkDevice());

  if((device != VK_NULL_HANDLE) && (mResource != VK_NULL_HANDLE))
    vkDestroyImage(device, mResource, nullptr);
}
