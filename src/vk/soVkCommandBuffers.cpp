/*
 * Copyright (C) 2017-2018 by Bennet Carstensen
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

#include "soVkCommandBuffers.hpp"

#include "cxx/soDebugCallback.hpp"
#include "cxx/soDefinitions.hpp"

so::vk::CommandBuffers::CommandBuffers()
  : mCommandBuffers(),
    mDevice(LogicalDevice::getSharedPtrNullDevice()),
    mCommandPool(CommandPool::getSharedPtrNullCommandPool())
{}

so::vk::CommandBuffers::~CommandBuffers() noexcept
{
  destroyMembers();
}

so::vk::CommandBuffers&
so::vk::CommandBuffers::operator=(CommandBuffers&& other) noexcept
{
  if(this is_eq &other)
  {
    return *this;
  }
          
  destroyMembers();

  mCommandBuffers = other.mCommandBuffers;
  mDevice         = other.mDevice;
  mCommandPool    = other.mCommandPool;
 
  other.mCommandBuffers = std::vector<VkCommandBuffer>();
  other.mDevice         = LogicalDevice::getSharedPtrNullDevice();
  other.mCommandPool    = CommandPool::getSharedPtrNullCommandPool();

  return *this;
}

so::return_t
so::vk::CommandBuffers::initialize(SharedPtrLogicalDevice const& device,
                                   SharedPtrCommandPool   const& commandPool,
                                   Framebuffers           const& framebuffers,
                                   RenderPass             const& renderPass,
                                   SwapChain              const& swapChain,
                                   Pipeline               const& pipeline)
{
  mDevice      = device;
  mCommandPool = commandPool;

  auto&         vkFramebuffers{ framebuffers.getVkFramebuffersRef() };

  VkCommandPool vkCommandPool{ mCommandPool->getVkCommandPool() };
  VkDevice      vkDevice{ mDevice->getVkDevice() };
  VkExtent2D    vkExtent{ swapChain.getVkExtent() };
  VkPipeline    vkPipeline{ pipeline.getVkPipeline() };
  VkRenderPass  vkRenderPass{ renderPass.getVkRenderPass() };
    
  mCommandBuffers.resize(vkFramebuffers.size());

  VkCommandBufferAllocateInfo allocInfo{};

  allocInfo.sType              =
    VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool        = vkCommandPool;
  allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = static_cast<uint32_t>(mCommandBuffers.size());

  VkResult result{ vkAllocateCommandBuffers(vkDevice,
                                            &allocInfo,
                                            mCommandBuffers.data()) };
  
  if(result not_eq VK_SUCCESS)
  {
    DEBUG_CALLBACK(error,
                   "Failed to allocate command buffers.",
                   vkAllocateCommandBuffers);

    return failure;
  }

  auto begin{ mCommandBuffers.begin() };
  auto end{ mCommandBuffers.end() };

  for(auto it{ begin }; it not_eq end; ++it)
  {
    VkCommandBufferBeginInfo beginInfo{};
    
    beginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags            = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(*it, &beginInfo) not_eq VK_SUCCESS)
    {
      DEBUG_CALLBACK(error,
                     "Failed to begin recording a command buffer.",
                     vkBeginCommandBuffer);
    
      return failure;
    }

    auto const idx{ static_cast<std::size_t>(std::distance(begin, it)) };

    VkRenderPassBeginInfo renderPassInfo{};

    renderPassInfo.sType             =
      VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass        = vkRenderPass;
    renderPassInfo.framebuffer       = vkFramebuffers[idx];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = vkExtent;

    VkClearValue clearColor{ 0.0f, 0.0f, 0.0f, 1.0f };

    renderPassInfo.clearValueCount   = 1;
    renderPassInfo.pClearValues      = &clearColor;

    vkCmdBeginRenderPass(*it,
                         &renderPassInfo,
                         VK_SUBPASS_CONTENTS_INLINE);
    
    vkCmdBindPipeline(*it,
                      VK_PIPELINE_BIND_POINT_GRAPHICS,
                      vkPipeline);

    vkCmdDraw(*it, 3, 1, 0, 0);

    vkCmdEndRenderPass(*it);

    if(vkEndCommandBuffer(*it) not_eq VK_SUCCESS)
    {
      DEBUG_CALLBACK(error,
                     "Failed to record command buffer.",
                     vkEndCommandBuffer);
    
      return failure;
    }
  }

  return success;
}

void
so::vk::CommandBuffers::destroyMembers()
{
  VkCommandPool commandPool(mCommandPool->getVkCommandPool());
  VkDevice      device(mDevice->getVkDevice());

  if(device not_eq VK_NULL_HANDLE)
  {
    if(commandPool not_eq VK_NULL_HANDLE)
    {
      if(not mCommandBuffers.empty())
      {
        vkFreeCommandBuffers(device,
                             commandPool,
                             static_cast<uint32_t>(mCommandBuffers.size()),
                             mCommandBuffers.data());
      }
    }
  }
}

