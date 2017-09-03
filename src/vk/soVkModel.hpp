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

/**
 *  @file      soVkModel.hpp
 *  @author    Bennet Carstensen
 *  @date      2017
 *  @copyright Copyright (c) 2017 Bennet Carstensen
 *
 *             Permission is hereby granted, free of charge, to any person
 *             obtaining a copy of this software and associated documentation
 *             files (the "Software"), to deal in the Software without
 *             restriction, including without limitation the rights to use,
 *             copy, modify, merge, publish, distribute, sublicense, and/or sell
 *             copies of the Software, and to permit persons to whom the
 *             Software is furnished to do so, subject to the following
 *             conditions:
 *
 *             The above copyright notice and this permission notice shall be
 *             included in all copies or substantial portions of the Software.
 *
 *             THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *             EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *             OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *             NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *             HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *             WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *             FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *             OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <vk/soVkDescriptorSet.hpp>
#include <vk/soVkMesh.hpp>

namespace so
{
  namespace vk
  {
    class
    Model
    {
      public:
        Model();

        Model(std::string const&            path,
              std::string const&            textureKey,
              std::size_t const             numInstances = 0);

        Model(Model const& other) = delete;

        Model(Model&& other) noexcept;

        Model& operator=(Model const& other) = delete;

        Model&
        operator=(Model&& other) noexcept
        {
          mDescriptorSet     = std::move(other.mDescriptorSet);
          mMesh              = std::move(other.mMesh);
          mNumInstances      = other.mNumInstances;
          mTextureKey        = other.mTextureKey;
          mDynamicUBOsOffset = other.mDynamicUBOsOffset;

          other.mNumInstances      = 0;
          other.mTextureKey        = "";
          other.mDynamicUBOsOffset = 0;

          return *this;
        }

        auto& getDescriptorSet() { return mDescriptorSet; }

        auto& getDescriptorSet() const { return mDescriptorSet; }

        auto& getMesh() { return mMesh; }

        auto& getMesh() const { return mMesh; }

        auto  getNumInstances() const { return mNumInstances; }

        auto& getTextureKey() { return mTextureKey; }

        auto  getDynamicUBOsOffset() { return mDynamicUBOsOffset; }

        auto  getDynamicUBOsOffset() const { return mDynamicUBOsOffset; }

        auto  setDynamicUBOsOffset(std::size_t const dynamicUBOOffset)
        { mDynamicUBOsOffset = dynamicUBOOffset; }

      private:
        DescriptorSet mDescriptorSet;
        Mesh          mMesh;
        std::size_t   mNumInstances;
        std::string   mTextureKey;
        std::size_t   mDynamicUBOsOffset;
    };

    using ModelMap = std::map<std::string, Model>;
  } // vk
} // so