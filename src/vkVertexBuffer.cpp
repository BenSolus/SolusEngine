/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vkVertexBuffer.hpp>

#include <vkModel.hpp>
#include <vkStagingBuffer.hpp>

vk::eng::VertexBuffer::VertexBuffer()
  : Buffer()
{}

vk::eng::VertexBuffer::VertexBuffer(SharedPtrLogicalDevice const& device,
                                    VkCommandPool                 commandPool)
  : Buffer(device,
           sizeof(Model::getVertices()[0]) * Model::getVertices().size(),
           VK_BUFFER_USAGE_TRANSFER_DST_BIT |
           VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
           VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
{
  VkDeviceSize  size(sizeof(Model::getVertices()[0]) *
                     Model::getVertices().size());

  StagingBuffer stagingBuffer(device, size, Model::getVertices().data());

  copyBuffer(stagingBuffer.getResource(), size, commandPool);
}
