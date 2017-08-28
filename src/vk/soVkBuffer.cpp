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

#include <vk/soVkBuffer.hpp>

#include <vk/soVkVertex.hpp>

so::vk::Buffer::Buffer() : Resource() {}

so::vk::Buffer::Buffer(SharedPtrLogicalDevice const& device,
                        VkDeviceSize                  size,
                        VkBufferUsageFlags            usage,
                        VkMemoryPropertyFlags         properties)
  : Resource(device)
{
  VkBufferCreateInfo bufferInfo = {};

  bufferInfo.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size        = size;
  bufferInfo.usage       = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VkDevice vkDevice(device->getVkDevice());

  if(vkCreateBuffer(vkDevice, &bufferInfo, nullptr, &mResource) != VK_SUCCESS)
    throw utils::err::Exception<std::runtime_error>("failed to create buffer!",
                                                    PRETTY_FUNCTION_SIG);


  VkMemoryRequirements memRequirements;

  vkGetBufferMemoryRequirements(vkDevice, mResource, &memRequirements);

  VkMemoryAllocateInfo allocInfo = {};

  allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize  = memRequirements.size;
  allocInfo.memoryTypeIndex =
    findMemoryType(memRequirements.memoryTypeBits, properties);

  VkResult const result(vkAllocateMemory(vkDevice,
                                         &allocInfo,
                                         nullptr,
                                         &mDeviceMemory));

  if(result not_eq VK_SUCCESS)
    throw utils::err::Exception<std::runtime_error>("failed to allocate buffer "
                                                    "memory!",
                                                    PRETTY_FUNCTION_SIG);

  vkBindBufferMemory(vkDevice, mResource, mDeviceMemory, 0);
}

so::vk::Buffer::~Buffer() noexcept { destroyMembers(); }

void
so::vk::Buffer::copyBuffer(VkBuffer      srcBuffer,
                            VkDeviceSize  size,
                            VkCommandPool commandPool)
{
  VkCommandBuffer commandBuffer = beginSingleTimeCommands(commandPool);

  VkBufferCopy copyRegion = {};

  copyRegion.size = size;

  vkCmdCopyBuffer(commandBuffer, srcBuffer, mResource, 1, &copyRegion);

  endSingleTimeCommands(commandPool, commandBuffer);
}

void
so::vk::Buffer::destroyMembers()
{
  VkDevice device(mDevice->getVkDevice());

  if((device != VK_NULL_HANDLE) && (mResource != VK_NULL_HANDLE))
    vkDestroyBuffer(device, mResource, nullptr);
}
