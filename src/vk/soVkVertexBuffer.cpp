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
