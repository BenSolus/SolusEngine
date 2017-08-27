/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vk/soVkCommandBuffers.hpp>

#include <vk/soVkMesh.hpp>

so::vk::CommandBuffers::CommandBuffers()
  : mCommandBuffers(),
    mIndexBuffer(),
    mVertexBuffer(),
    mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE),
    mCommandPool(SHARED_PTR_NULL_COMMAND_POOL)
{}

so::vk::CommandBuffers::CommandBuffers
  (SharedPtrLogicalDevice const& device,
   SharedPtrCommandPool const&   commandPool,
   std::vector<VkFramebuffer>&   framebuffers,
   VkRenderPass                  renderPass,
   VkDescriptorSet&              descriptorSet,
   VkExtent2D                    extent,
   VkPipeline                    pipeline,
   VkPipelineLayout              pipelineLayout,
   UniformBuffer&                uniformBuffer)
  : mCommandBuffers(),
    mIndexBuffer(device, commandPool),
    mVertexBuffer(device, commandPool),
    mDevice(device),
    mCommandPool(commandPool)
{
  mCommandBuffers.resize(framebuffers.size());

  VkCommandBufferAllocateInfo allocInfo({});

  allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool        = mCommandPool->getVkCommandPool();
  allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = static_cast<uint32_t>(mCommandBuffers.size());

  VkResult const result(vkAllocateCommandBuffers(device->getVkDevice(),
                                                 &allocInfo,
                                                 mCommandBuffers.data()));

  if(result not_eq VK_SUCCESS)
    throw utils::err::Exception<std::runtime_error>("failed to allocate "
                                                    "command buffers!",
                                                    PRETTY_FUNCTION_SIG);

  size_t i(0);

  for(auto const& commandBuffers : mCommandBuffers)
  {
    VkCommandBufferBeginInfo beginInfo({});

    beginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags            = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    beginInfo.pInheritanceInfo = nullptr;

    vkBeginCommandBuffer(commandBuffers, &beginInfo);

    VkRenderPassBeginInfo renderPassInfo({});

    renderPassInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass        = renderPass;
    renderPassInfo.framebuffer       = framebuffers[i];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = extent;

    std::array<VkClearValue, 2> clearValues({});

    clearValues[0].color        = { 0.0f, 0.0f, 0.0f, 1.0f };
    clearValues[1].depthStencil = { 1.0f, 0 };

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues    = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers,
                         &renderPassInfo,
                         VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffers,
                      VK_PIPELINE_BIND_POINT_GRAPHICS,
                      pipeline);

    VkBuffer vertexBuffers[]{ mVertexBuffer.getResource() };

    VkDeviceSize offsets[]{ 0 };

    vkCmdBindVertexBuffers(commandBuffers, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(commandBuffers,
                         mIndexBuffer.getResource(),
                         0,
                         VK_INDEX_TYPE_UINT32);

    for(std::size_t j(0); j < uniformBuffer.getDynamicUBOs().size(); ++j)
    {
      const uint32_t dynamicOffset
        (static_cast<uint32_t>(j * uniformBuffer.getDynamicUBOs().alignment()));

      vkCmdBindDescriptorSets(commandBuffers,
                              VK_PIPELINE_BIND_POINT_GRAPHICS,
                              pipelineLayout,
                              0,
                              1,
                              &descriptorSet,
                              1,
                              &dynamicOffset);

      vkCmdDrawIndexed(commandBuffers,
                       static_cast<uint32_t>(Mesh::getIndices().size()),
                       1,
                       0,
                       0,
                       0);
    }

    vkCmdEndRenderPass(commandBuffers);

    if(vkEndCommandBuffer(commandBuffers) not_eq VK_SUCCESS)
      throw utils::err::Exception<std::runtime_error>("failed to record "
                                                      "command buffer!",
                                                      PRETTY_FUNCTION_SIG);

    ++i;
  }
}

so::vk::CommandBuffers::~CommandBuffers() noexcept
{
  destroyMembers();
}

void
so::vk::CommandBuffers::destroyMembers()
{
  VkCommandPool commandPool(mCommandPool->getVkCommandPool());
  VkDevice      device(mDevice->getVkDevice());

  if(device not_eq VK_NULL_HANDLE)
    if(commandPool not_eq VK_NULL_HANDLE)
      if(mCommandBuffers.size() not_eq 0)
        vkFreeCommandBuffers(device,
                             commandPool,
                             static_cast<uint32_t>(mCommandBuffers.size()),
                             mCommandBuffers.data());
}
