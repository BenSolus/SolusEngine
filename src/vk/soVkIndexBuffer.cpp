/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vk/soVkIndexBuffer.hpp>

#include <vk/soVkMesh.hpp>
#include <vk/soVkStagingBuffer.hpp>

so::vk::IndexBuffer::IndexBuffer()
  : Buffer()
{}

so::vk::IndexBuffer::IndexBuffer(SharedPtrLogicalDevice const& device,
                                  SharedPtrCommandPool const&   commandPool)
  : Buffer(device,
           sizeof(Mesh::getIndices()[0]) * Mesh::getIndices().size(),
           VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
           VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
{
  VkDeviceSize  size(sizeof(Mesh::getIndices()[0]) *
                       Mesh::getIndices().size());

  StagingBuffer stagingBuffer(device, size, Mesh::getIndices().data());

  copyBuffer(stagingBuffer.getResource(),
             size,
             commandPool->getVkCommandPool());
}
