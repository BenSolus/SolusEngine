/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      Model.hpp
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

#include <vk/soVkDescriptorSet.hpp>
#include <vk/soVkMesh.hpp>

namespace so
{
  namespace vk
  {
    class
    soVkModel {
      public:
        soVkModel();

      private:
        DescriptorSet mDescriptorSet;
        Mesh          mMesh;

        std::size_t   mNumInstances;
        std::string   mTextureKey;
    };
  } // vk
} // so