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

#include <vkCommandPool.hpp>
#include <vkLogicalDevice.hpp>

namespace vk
{
  namespace eng
  {
    class
    CommandBuffers
    {
      public:
        CommandBuffers();

        CommandBuffers(SharedPtrLogicalDevice const& device,
                       SharedPtrCommandPool   const& commandPool,
                       std::vector<VkFramebuffer>&   framebuffers,
                       VkRenderPass                  renderPass,
                       VkDescriptorSet&              descriptorSet,
                       VkExtent2D                    extent,
                       VkPipeline                    pipeline,
                       VkPipelineLayout              pipelineLayout,
                       VkBuffer                      vertexBuffer,
                       VkBuffer                      indexBuffer);

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
          mCommandPool    = other.mCommandPool;
          mDevice         = other.mDevice;

          other.mCommandBuffers = std::vector<VkCommandBuffer>();
          other.mCommandPool    = SHARED_PTR_NULL_COMMAND_POOL;
          other.mDevice         = SHARED_PTR_NULL_LOGICAL_DEVICE;

          return *this;
        }

        inline std::vector<VkCommandBuffer>& getVkCommandBuffers()
        { return mCommandBuffers; }

        inline SharedPtrCommandPool getCommandPool()
        { return mCommandPool->shared_from_this(); }

      private:
        std::vector<VkCommandBuffer> mCommandBuffers;

        SharedPtrCommandPool         mCommandPool;
        SharedPtrLogicalDevice       mDevice;

        void
        destroyMembers();
    };
  } // namespace eng
} // namespace vk
