/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vkBuffer.hpp>

#include <vkVertex.hpp>

vk::eng::Buffer::Buffer() : Resource() {}

vk::eng::Buffer::Buffer(SharedPtrLogicalDevice const& device,
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
    cc::throw_with_nested<std::runtime_error>("failed to create buffer!",
                                              PRETTY_FUNCTION_SIG);


  VkMemoryRequirements memRequirements;

  vkGetBufferMemoryRequirements(vkDevice, mResource, &memRequirements);

  VkMemoryAllocateInfo allocInfo = {};

  allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize  = memRequirements.size;
  allocInfo.memoryTypeIndex =
    findMemoryType(memRequirements.memoryTypeBits, properties);

  if(vkAllocateMemory(vkDevice, &allocInfo, nullptr, &mDeviceMemory) != VK_SUCCESS)
  {
    cc::throw_with_nested<std::runtime_error>("failed to allocate buffer "
                                              "memory!",
                                              PRETTY_FUNCTION_SIG);
  }

  vkBindBufferMemory(vkDevice, mResource, mDeviceMemory, 0);
}

vk::eng::Buffer::~Buffer() noexcept { destroyMembers(); }

void
vk::eng::Buffer::copyBuffer(VkBuffer      srcBuffer,
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
vk::eng::Buffer::destroyMembers()
{
  VkDevice device(mDevice->getVkDevice());

  if((device != VK_NULL_HANDLE) && (mResource != VK_NULL_HANDLE))
    vkDestroyBuffer(device, mResource, nullptr);
}
