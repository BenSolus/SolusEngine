/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vk/soVkVertexBuffer.hpp>

#include <vk/soVkMesh.hpp>
#include <vk/soVkStagingBuffer.hpp>

so::vk::VertexBuffer::VertexBuffer()
  : Buffer()
{}

so::vk::VertexBuffer::VertexBuffer(SharedPtrLogicalDevice const& device,
                                    SharedPtrCommandPool const&   commandPool)
  : Buffer(device,
           sizeof(Mesh::getVertices()[0]) * Mesh::getVertices().size(),
           VK_BUFFER_USAGE_TRANSFER_DST_BIT |
           VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
           VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
{
  VkDeviceSize  size(sizeof(Mesh::getVertices()[0]) *
                     Mesh::getVertices().size());

  StagingBuffer stagingBuffer(device, size, Mesh::getVertices().data());

  copyBuffer(stagingBuffer.getResource(),
             size,
             commandPool->getVkCommandPool());
}
