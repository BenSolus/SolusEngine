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

#include <vk/soVkRenderPass.hpp>

so::vk::RenderPass::RenderPass()
  : mRenderPass(VK_NULL_HANDLE), mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE)
{}

so::vk::RenderPass::RenderPass
  (SharedPtrLogicalDevice const& device,
   SwapChain&                    swapChain,
   DepthImage&                   depthImage)
  : mRenderPass(VK_NULL_HANDLE), mDevice(device)
{
  VkAttachmentDescription colorAttachment({});

  colorAttachment.format         = swapChain.getVkFormat();
  colorAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentDescription depthAttachment({});

  depthAttachment.format         = depthImage.getVkFormat();
  depthAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
  depthAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
  depthAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
  depthAttachment.finalLayout    =
    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentReference *colorAttachmentRef(new VkAttachmentReference{});

  colorAttachmentRef->attachment = 0;
  colorAttachmentRef->layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkAttachmentReference *depthAttachmentRef(new VkAttachmentReference{});

  depthAttachmentRef->attachment = 1;
  depthAttachmentRef->layout     =
    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkSubpassDescription *subpass(new VkSubpassDescription({}));

  subpass->pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass->colorAttachmentCount    = 1;
  subpass->pColorAttachments       = colorAttachmentRef;
  subpass->pDepthStencilAttachment = depthAttachmentRef;

  VkSubpassDependency *dependency(new VkSubpassDependency({}));

  dependency->srcSubpass    = VK_SUBPASS_EXTERNAL;
  dependency->dstSubpass    = 0;
  dependency->srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency->srcAccessMask = 0;
  dependency->dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency->dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
                              VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  std::array<VkAttachmentDescription, 2> attachments({ colorAttachment,
                                                       depthAttachment });

  VkRenderPassCreateInfo renderPassInfo({});

  renderPassInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
  renderPassInfo.pAttachments    = attachments.data();
  renderPassInfo.subpassCount    = 1;
  renderPassInfo.pSubpasses      = subpass;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies   = dependency;

  VkResult const result(vkCreateRenderPass(device->getVkDevice(),
                                           &renderPassInfo,
                                           nullptr,
                                           &mRenderPass));

  delete dependency;
  delete subpass;
  delete depthAttachmentRef;
  delete colorAttachmentRef;

  if(result not_eq VK_SUCCESS)
    throw utils::err::Exception<std::runtime_error>("failed to create render "
                                                    "pass!",
                                                    PRETTY_FUNCTION_SIG);
}

so::vk::RenderPass::~RenderPass() noexcept
{
  destroyMembers();
}

void
so::vk::RenderPass::destroyMembers()
{
  VkDevice device(mDevice->getVkDevice());

  if((mRenderPass != VK_NULL_HANDLE) && (device != VK_NULL_HANDLE))
    vkDestroyRenderPass(device, mRenderPass, nullptr);
}
