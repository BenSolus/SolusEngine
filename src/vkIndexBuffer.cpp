/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vkIndexBuffer.hpp>

#include <vkModel.hpp>
#include <vkStagingBuffer.hpp>

vk::eng::IndexBuffer::IndexBuffer()
  : Buffer()
{}

vk::eng::IndexBuffer::IndexBuffer(SharedPtrLogicalDevice const& device,
                                  VkCommandPool                 commandPool)
  : Buffer(device,
           sizeof(Model::getIndices()[0]) * Model::getIndices().size(),
           VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
           VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
{
  VkDeviceSize  size(sizeof(Model::getIndices()[0]) *
                     Model::getIndices().size());

  StagingBuffer stagingBuffer(device, size, Model::getIndices().data());

  copyBuffer(stagingBuffer.getResource(), size, commandPool);
}
