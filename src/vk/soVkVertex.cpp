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

#include <vk/soVkVertex.hpp>

so::vk::Vertex::Vertex(glm::vec3 position,
                        glm::vec3 color,
                        glm::vec2 texCoord)
  : mPosition(position), mColor(color), mTexCoord(texCoord)
{}

VkVertexInputBindingDescription
so::vk::Vertex::getBindingDescription()
{
  VkVertexInputBindingDescription bindingDescription = {};

  bindingDescription.binding   = 0;
  bindingDescription.stride    = sizeof(Vertex);
  bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

  return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 3>
so::vk::Vertex::getAttributeDescriptions()
{
  std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

  attributeDescriptions[0].binding  = 0;
  attributeDescriptions[0].location = 0;
  attributeDescriptions[0].format   = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[0].offset   = static_cast<uint32_t>
                                        (offsetof(Vertex, mPosition));

  attributeDescriptions[1].binding  = 0;
  attributeDescriptions[1].location = 1;
  attributeDescriptions[1].format   = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[1].offset   = static_cast<uint32_t>
                                        (offsetof(Vertex, mColor));

  attributeDescriptions[2].binding  = 0;
  attributeDescriptions[2].location = 2;
  attributeDescriptions[2].format   = VK_FORMAT_R32G32_SFLOAT;
  attributeDescriptions[2].offset   = static_cast<uint32_t>
                                        (offsetof(Vertex, mTexCoord));

  return attributeDescriptions;
}
