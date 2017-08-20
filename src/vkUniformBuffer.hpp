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

#include <vkBuffer.hpp>

#include <ccMemory.hpp>

#include <glm/glm.hpp>

namespace vk
{
  namespace eng
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
    UniformBuffer : public Buffer
    {
      public:
        UniformBuffer();

        UniformBuffer(SharedPtrLogicalDevice const& device,
                      const std::size_t             dynamicAlignment,
                      const std::size_t             numDynamicUBOs);

        UniformBuffer(UniformBuffer const& other) = delete;

        UniformBuffer(UniformBuffer&& other) = delete;

        ~UniformBuffer() noexcept;

        UniformBuffer& operator=(UniformBuffer const& other) = delete;

        UniformBuffer&
        operator=(UniformBuffer&& other) noexcept
        {
          Buffer::operator=(static_cast<Buffer&&>(other));

          if(mDynamicUBOs not_eq nullptr)
            cc::alignedFree(mDynamicUBOs);

          mViewProjectionUBO = other.mViewProjectionUBO;
          mNumDynamicUBOs    = other.mNumDynamicUBOs;
          mDynamicUBOs       = other.mDynamicUBOs;
          mDynamicAlignment  = other.mDynamicAlignment;

          other.mViewProjectionUBO = {};
          other.mNumDynamicUBOs    = 0;
          other.mDynamicUBOs       = nullptr;
          other.mDynamicAlignment  = 0;

          mDynamicBuffer = std::move(other.mDynamicBuffer);

          return *this;
        }

        inline auto getDynamicAlignment() { return mDynamicAlignment; }

        inline auto getNumberOfDynamicUBOs() { return mNumDynamicUBOs; }

        inline auto& getDynamicBuffer() { return mDynamicBuffer; }
        void
        updateViewProjectionUBO(VkExtent2D swapChainExtent);

        void
        update();

      private:
        ViewProjectionUBO mViewProjectionUBO;

        std::size_t       mDynamicAlignment;

        std::size_t       mNumDynamicUBOs;

        DynamicUBO*       mDynamicUBOs;

        Buffer            mDynamicBuffer;
    };
  } // namespace eng
} // namespace vk
