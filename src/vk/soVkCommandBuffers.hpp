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
 *  @file      soVkCommandBuffer.hpp
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
