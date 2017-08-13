/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vkCommandBuffers.hpp>

#include <vkModel.hpp>

#include <ccException.hpp>

vk::eng::CommandBuffers::CommandBuffers()
  : mCommandBuffers(),
    mCommandPool(SHARED_PTR_NULL_COMMAND_POOL),
    mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE)
{}

vk::eng::CommandBuffers::CommandBuffers
  (SharedPtrLogicalDevice const& device,
   SharedPtrCommandPool   const& commandPool,
   std::vector<VkFramebuffer>&   framebuffers,
   VkRenderPass                  renderPass,
   VkDescriptorSet&              descriptorSet,
   VkExtent2D                    extent,
   VkPipeline                    pipeline,
   VkPipelineLayout              pipelineLayout,
   VkBuffer                      vertexBuffer,
   VkBuffer                      indexBuffer)
  : mCommandBuffers(), mCommandPool(commandPool), mDevice(device)
{
  mCommandBuffers.resize(framebuffers.size());

  VkCommandBufferAllocateInfo allocInfo = {};

  allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool        = commandPool->getVkCommandPool();
  allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = static_cast<uint32_t>(mCommandBuffers.size());

  if(vkAllocateCommandBuffers(device->getVkDevice(), &allocInfo, mCommandBuffers.data()) != VK_SUCCESS)
  {
    cc::throw_with_nested<std::runtime_error>("failed to allocate command "
                                              "buffers!",
                                              PRETTY_FUNCTION_SIG);
  }

  for(size_t i(0); i < mCommandBuffers.size(); ++i)
  {
    VkCommandBufferBeginInfo beginInfo = {};

    beginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags            = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    beginInfo.pInheritanceInfo = nullptr;

    vkBeginCommandBuffer(mCommandBuffers[i], &beginInfo);

    VkRenderPassBeginInfo renderPassInfo = {};

    renderPassInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass        = renderPass;
    renderPassInfo.framebuffer       = framebuffers[i];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = extent;

    std::array<VkClearValue, 2> clearValues = {};

    clearValues[0].color        = { 0.0f, 0.0f, 0.0f, 1.0f };
    clearValues[1].depthStencil = { 1.0f, 0 };

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues    = clearValues.data();

    vkCmdBeginRenderPass(mCommandBuffers[i],
                         &renderPassInfo,
                         VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(mCommandBuffers[i],
                      VK_PIPELINE_BIND_POINT_GRAPHICS,
                      pipeline);

    VkBuffer vertexBuffers[] = { vertexBuffer };

    VkDeviceSize offsets[] = { 0 };

    vkCmdBindVertexBuffers(mCommandBuffers[i], 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(mCommandBuffers[i],
                         indexBuffer,
                         0,
                         VK_INDEX_TYPE_UINT32);

    vkCmdBindDescriptorSets(mCommandBuffers[i],
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            pipelineLayout,
                            0,
                            1,
                            &descriptorSet,
                            0,
                            nullptr);

    vkCmdDrawIndexed(mCommandBuffers[i],
                     static_cast<uint32_t>(Model::getIndices().size()), 1, 0, 0, 0);

    vkCmdEndRenderPass(mCommandBuffers[i]);

    if(vkEndCommandBuffer(mCommandBuffers[i]) != VK_SUCCESS)
      cc::throw_with_nested<std::runtime_error>("failed to record command "
                                                "buffer!",
                                                PRETTY_FUNCTION_SIG);
  }
}

vk::eng::CommandBuffers::~CommandBuffers() noexcept
{
  destroyMembers();
}

void
vk::eng::CommandBuffers::destroyMembers()
{
  VkCommandPool commandPool(mCommandPool->getVkCommandPool());
  VkDevice      device(mDevice->getVkDevice());

  if((device != VK_NULL_HANDLE) && (commandPool != VK_NULL_HANDLE) && (mCommandBuffers.size() > 0))
  {
    vkFreeCommandBuffers(device,
                         commandPool,
                         static_cast<uint32_t>(mCommandBuffers.size()),
                         mCommandBuffers.data());
  }
}
