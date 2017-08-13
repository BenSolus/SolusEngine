/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkVertex.hpp
 *  @author    Bennet Carstensen
 *  @date      2017
 *  @copyright Copyright (c) 2017 Bennet Carstensen
 *
 *             This Source Code Form is subject to the terms of the
 *             Mozilla Public License, v. 2.0. If a copy of the MPL was
 *             not distributed with this file, You can obtain one at
 *             http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <vulkan/vulkan.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <array>
#include <vector>

namespace vk
{
  namespace eng
  {
    class
    Vertex
    {
      public:
        Vertex(glm::vec3 position, glm::vec3 color, glm::vec2 texCoord);

        bool
        operator==(const Vertex& other) const
        {
          return mPosition == other.mPosition &&
                 mColor == other.mColor &&
                 mTexCoord == other.mTexCoord;
        }

        static VkVertexInputBindingDescription
        getBindingDescription();

        static std::array<VkVertexInputAttributeDescription, 3>
        getAttributeDescriptions();

        glm::vec3 const& getPosition() const { return mPosition; }

        glm::vec3 const& getColor() const { return mColor; }

        glm::vec2 const& getTexCoord() const { return mTexCoord; }
      private:
        glm::vec3 mPosition;
        glm::vec3 mColor;
        glm::vec2 mTexCoord;
    };
  } // namespace vk
} // namespace fcm

namespace std
{
  template<>
  struct hash<vk::eng::Vertex>
  {
    size_t
    operator()(vk::eng::Vertex const& vertex) const
    {
      return ((hash<glm::vec3>()(vertex.getPosition()) ^
               (hash<glm::vec3>()(vertex.getColor()) << 1)) >> 1) ^
             (hash<glm::vec2>()(vertex.getTexCoord()) << 1);
    }
  };
} // namespace std
