/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkCommandBuffer.hpp
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

#include <vk/soVkCommandPool.hpp>
#include <vk/soVkIndexBuffer.hpp>
#include <vk/soVkUniformBuffer.hpp>
#include <vk/soVkVertexBuffer.hpp>

namespace so
{
  namespace vk
  {
    class
    CommandBuffers
    {
      public:
        CommandBuffers();

        CommandBuffers(SharedPtrLogicalDevice const& device,
                       SharedPtrCommandPool const&   commandPool,
                       std::vector<VkFramebuffer>&   framebuffers,
                       VkRenderPass                  renderPass,
                       VkDescriptorSet&              descriptorSet,
                       VkExtent2D                    extent,
                       VkPipeline                    pipeline,
                       VkPipelineLayout              pipelineLayout,
                       UniformBuffer&                uniformBuffer);

        CommandBuffers(CommandBuffers const& other) = delete;

        CommandBuffers(CommandBuffers&& other) = delete;

        ~CommandBuffers() noexcept;

        CommandBuffers&
        operator=(CommandBuffers const& other) = delete;

        CommandBuffers&
        operator=(CommandBuffers&& other) noexcept
        {
          destroyMembers();

          mCommandBuffers = other.mCommandBuffers;
          mDevice         = other.mDevice;
          mCommandPool    = other.mCommandPool;

          mIndexBuffer    = std::move(other.mIndexBuffer);
          mVertexBuffer   = std::move(other.mVertexBuffer);

          other.mCommandBuffers = std::vector<VkCommandBuffer>();
          other.mDevice         = SHARED_PTR_NULL_LOGICAL_DEVICE;
          other.mCommandPool    = SHARED_PTR_NULL_COMMAND_POOL;

          return *this;
        }

        inline auto& getVkCommandBuffers() { return mCommandBuffers; }

      private:
        std::vector<VkCommandBuffer> mCommandBuffers;

        IndexBuffer                  mIndexBuffer;
        VertexBuffer                 mVertexBuffer;

        SharedPtrLogicalDevice       mDevice;
        SharedPtrCommandPool         mCommandPool;

        void
        destroyMembers();
    };
  } // namespace vk
} // namespace so
