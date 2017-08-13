/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vkStagingBuffer.hpp>

#include <vkVertex.hpp>

vk::eng::StagingBuffer::StagingBuffer()
  : Buffer()
{}

vk::eng::StagingBuffer::StagingBuffer(SharedPtrLogicalDevice const& device,
                                      VkDeviceSize                  size,
                                      void const*                   res)
  : Buffer(device,
           size,
           VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
           VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{

  VkDevice     vkDevice(device->getVkDevice());
  VkDeviceSize bufferSize(size);

  void* data;

  vkMapMemory(vkDevice, mDeviceMemory, 0, bufferSize, 0, &data);

  memcpy(data, res, static_cast<std::size_t>(bufferSize));

  vkUnmapMemory(vkDevice, mDeviceMemory);
}
