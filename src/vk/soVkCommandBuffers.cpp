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

#include <vk/soVkCommandBuffers.hpp>

so::vk::CommandBuffers::CommandBuffers()
  : mCommandBuffers(),
    mIndexBuffer(),
    mVertexBuffer(),
    mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE),
    mCommandPool(SHARED_PTR_NULL_COMMAND_POOL)
{}

so::vk::CommandBuffers::CommandBuffers
  (SharedPtrLogicalDevice const& device,
   SharedPtrCommandPool const&   commandPool)
  : mCommandBuffers(),
    mIndexBuffer(device, commandPool),
    mVertexBuffer(device, commandPool),
    mDevice(device),
    mCommandPool(commandPool)
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
so::vk::CommandBuffers::recreateCommandBuffers
  (Framebuffers&                 framebuffers,
   Pipeline&                     pipeline,
   RenderPass&                   renderPass,
   SwapChain&                    swapChain,
   UniformBuffer&                uniformBuffer,
   ModelMap const&               models)
{
  destroyMembers();

  mIndexBuffer  = vk::IndexBuffer(mDevice, mCommandPool);
  mVertexBuffer = vk::VertexBuffer(mDevice, mCommandPool);

  if(Mesh::getVertices().size() not_eq 0)
  {
    mCommandBuffers.resize(framebuffers.getVkFramebuffers().size());

    VkCommandBufferAllocateInfo allocInfo({});

    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = mCommandPool->getVkCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(mCommandBuffers.size());

    VkResult const result(vkAllocateCommandBuffers(mDevice->getVkDevice(),
                                                   &allocInfo,
                                                   mCommandBuffers.data()));

    if (result not_eq VK_SUCCESS)
      throw utils::err::Exception<std::runtime_error>("failed to allocate "
                                                        "command buffers!",
                                                      PRETTY_FUNCTION_SIG);

    size_t i(0);

    for(auto const &commandBuffers : mCommandBuffers)
    {
      VkCommandBufferBeginInfo beginInfo({});

      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
      beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
      beginInfo.pInheritanceInfo = nullptr;

      vkBeginCommandBuffer(commandBuffers, &beginInfo);

      VkRenderPassBeginInfo renderPassInfo({});

      renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassInfo.renderPass = renderPass.getVkRenderPass();
      renderPassInfo.framebuffer = framebuffers.getVkFramebuffers()[i];
      renderPassInfo.renderArea.offset = {0, 0};
      renderPassInfo.renderArea.extent = swapChain.getVkExtent();

      std::array<VkClearValue, 2> clearValues({});

      clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
      clearValues[1].depthStencil = {1.0f, 0};

      renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
      renderPassInfo.pClearValues = clearValues.data();

      vkCmdBeginRenderPass(commandBuffers,
                           &renderPassInfo,
                           VK_SUBPASS_CONTENTS_INLINE);

      vkCmdBindPipeline(commandBuffers,
                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                        pipeline.getVkPipeline());

      VkBuffer vertexBuffers[]{mVertexBuffer.getResource()};

      VkDeviceSize offsets[]{0};

      vkCmdBindVertexBuffers(commandBuffers, 0, 1, vertexBuffers, offsets);

      vkCmdBindIndexBuffer(commandBuffers,
                           mIndexBuffer.getResource(),
                           0,
                           VK_INDEX_TYPE_UINT32);

      for (auto &model: models) {
        uint32_t dynamicOffset
          (static_cast<uint32_t>
           (model.second.getDynamicUBOsOffset() *
            uniformBuffer.getDynamicUBOs().alignment()));

        for (std::size_t j(0); j < model.second.getNumInstances(); ++j) {
          vkCmdBindDescriptorSets
            (commandBuffers,
             VK_PIPELINE_BIND_POINT_GRAPHICS,
             pipeline.getVkPipelineLayout(),
             0,
             1,
             &model.second.getDescriptorSet().getVkDescriptorSet(),
             1,
             &dynamicOffset);

          dynamicOffset += static_cast<uint32_t>
          (uniformBuffer.getDynamicUBOs().alignment());

          vkCmdDrawIndexed
            (commandBuffers,
             static_cast<uint32_t>(model.second.getMesh().getNumIndices()),
             1,
             static_cast<uint32_t>(model.second.getMesh().getIndexOffset()),
             static_cast<uint32_t>(model.second.getMesh().getVertexOffset()),
             0);
        }
      }

      vkCmdEndRenderPass(commandBuffers);

      if (vkEndCommandBuffer(commandBuffers) not_eq VK_SUCCESS)
        throw utils::err::Exception<std::runtime_error>("failed to record "
                                                          "command buffer!",
                                                        PRETTY_FUNCTION_SIG);

      ++i;
    }
  }
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
