/*
 * Copyright (C) 2017-2018 by Bennet Carstensen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "soVkRenderPass.hpp"

#include "cxx/soDebugCallback.hpp"
#include "cxx/soDefinitions.hpp"

so::vk::RenderPass::RenderPass()
  : mRenderPass(VK_NULL_HANDLE),
    mDevice(LogicalDevice::getSharedPtrNullDevice())
{}

so::vk::RenderPass::~RenderPass() noexcept
{
  destroyMembers();
}

so::vk::RenderPass&
so::vk::RenderPass::operator=(RenderPass &&other) noexcept
{
  if(this is_eq &other)
  {
    return *this;
  }

  destroyMembers();

  mRenderPass = other.mRenderPass;
  mDevice     = other.mDevice;

  other.mRenderPass = VK_NULL_HANDLE;
  other.mDevice     = LogicalDevice::getSharedPtrNullDevice();

  return *this;
}

so::return_t
so::vk::RenderPass::initialize(SharedPtrLogicalDevice const& device,
                               SwapChain              const& swapChain)
{
  mDevice = device;
 
  VkAttachmentDescription colorAttachment{};

  colorAttachment.format  			 = swapChain.getVkFormat();
  colorAttachment.samples 			 = VK_SAMPLE_COUNT_1_BIT;  
  colorAttachment.loadOp  			 = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp 			 = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout 	 = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};

	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout 		= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  
	VkSubpassDescription subpass{};
  
	subpass.pipelineBindPoint 	 = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments 	 = &colorAttachmentRef;
  
	VkRenderPassCreateInfo renderPassInfo{};

  renderPassInfo.sType 					 = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = 1;
  renderPassInfo.pAttachments 	 = &colorAttachment;
  renderPassInfo.subpassCount 	 = 1;
  renderPassInfo.pSubpasses 		 = &subpass;

	auto vkDevice{ mDevice->getVkDevice() };
 
	auto result{ vkCreateRenderPass(vkDevice,
																	&renderPassInfo,
																	nullptr,
																	&mRenderPass) };

  if(result not_eq VK_SUCCESS)
  {
		std::string message{ "Failed to create render pass." };

		PREPEND_FUNCTION_SIG_TO_STRING(message);

		executeDebugCallback(error, message);
  }

	return success;
}


void
so::vk::RenderPass::destroyMembers()
{
  VkDevice device(mDevice->getVkDevice());

  if((mRenderPass not_eq VK_NULL_HANDLE) and (device not_eq VK_NULL_HANDLE))
  {
    vkDestroyRenderPass(device, mRenderPass, nullptr);
  }
}

