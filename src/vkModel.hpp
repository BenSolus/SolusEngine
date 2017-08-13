/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkDepthImage.hpp
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

#include <vkVertex.hpp>

namespace vk
{
  namespace eng
  {
    class
    Model
    {
      public:
        Model();

        Model(std::string const& path);

        static inline std::vector<Vertex> const& getVertices()
        { return mVertices; }

        static inline std::vector<uint32_t> const& getIndices()
        { return mIndices; }

      private:
        static std::vector<Vertex>   mVertices;

        static std::vector<uint32_t> mIndices;
    };
  } // namespace eng
} // namespace vk
