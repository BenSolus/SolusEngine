/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkUniformBuffer.hpp
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
