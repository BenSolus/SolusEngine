/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
