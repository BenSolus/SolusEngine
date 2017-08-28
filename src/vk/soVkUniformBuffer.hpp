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
 *  @file      soVkUniformBuffer.hpp
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

#include <vk/soVkBuffer.hpp>

#include <utils/mem/soAlignedVector.hpp>

#include <glm/glm.hpp>

namespace so
{
  namespace vk
  {
    struct ViewProjectionUBO
    {
      glm::mat4 view;
      glm::mat4 proj;
    };

    struct DynamicUBO
    {
        glm::mat4 model;
    };

    class
    UniformBuffer
    {
      public:
        using DynamicUBOs = utils::mem::AlignedVector<DynamicUBO>;

      public:
        UniformBuffer();

        UniformBuffer(SharedPtrLogicalDevice const& device,
                      const std::size_t             dynamicAlignment,
                      const std::size_t             numDynamicUBOs);

        UniformBuffer(UniformBuffer const& other) = delete;

        UniformBuffer(UniformBuffer&& other) = delete;

        UniformBuffer& operator=(UniformBuffer const& other) = delete;

        UniformBuffer&
        operator=(UniformBuffer&& other) noexcept
        {
          mViewProjectionUBO       = other.mViewProjectionUBO;
          mViewProjectionUBOBuffer = std::move(other.mViewProjectionUBOBuffer);
          mDynamicUBOs             = std::move(other.mDynamicUBOs);
          mDynamicBuffer           = std::move(other.mDynamicBuffer);
          mDevice                  = other.mDevice;

          other.mViewProjectionUBO = {};
          other.mDevice            = SHARED_PTR_NULL_LOGICAL_DEVICE;

          return *this;
        }

        inline auto&
        getViewProjectionUBOBuffer()
        { return mViewProjectionUBOBuffer; }

        inline auto& getDynamicUBOs() {return mDynamicUBOs; }

        inline auto& getDynamicBuffer() { return mDynamicBuffer; }

        void
        updateViewProjectionUBO(VkExtent2D swapChainExtent);

        void
        update();

      private:
        ViewProjectionUBO      mViewProjectionUBO;

        Buffer                 mViewProjectionUBOBuffer;

        DynamicUBOs            mDynamicUBOs;

        Buffer                 mDynamicBuffer;

        SharedPtrLogicalDevice mDevice;
    };
  } // namespace vk
} // namespace so
