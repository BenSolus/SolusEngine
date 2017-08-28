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

#include <vk/soVkFramebuffers.hpp>

#include <utils/err/soException.hpp>

so::vk::Framebuffers::Framebuffers()
  : mFramebuffers(), mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE)
{}

so::vk::Framebuffers::Framebuffers
  (SharedPtrLogicalDevice const& device,
   std::vector<VkImageView>&     swapChainImageViews,
   VkRenderPass                  renderPass,
   VkExtent2D                    swapChainExtent,
   VkImageView                   depthImageView)
  : mFramebuffers(), mDevice(device)
{
  mFramebuffers.resize(swapChainImageViews.size());

  for(size_t i(0); i < swapChainImageViews.size(); ++i)
  {
    std::array<VkImageView, 2> attachments = { swapChainImageViews[i],
                                               depthImageView };

    VkFramebufferCreateInfo framebufferInfo = {};

    framebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass      = renderPass;
    framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    framebufferInfo.pAttachments    = attachments.data();
    framebufferInfo.width           = swapChainExtent.width;
    framebufferInfo.height          = swapChainExtent.height;
    framebufferInfo.layers          = 1;

    VkResult const result(vkCreateFramebuffer(device->getVkDevice(),
                                              &framebufferInfo,
                                              nullptr,
                                              &mFramebuffers[i]));

    if(result not_eq VK_SUCCESS)
      throw utils::err::Exception<std::runtime_error>("failed to create "
                                                      "framebuffer!",
                                                      PRETTY_FUNCTION_SIG);
  }
}

so::vk::Framebuffers::~Framebuffers() noexcept { destroyMembers(); }

void
so::vk::Framebuffers::destroyMembers()
{
  VkDevice device(mDevice->getVkDevice());

  if(device != VK_NULL_HANDLE)
    for(size_t i(0); i < mFramebuffers.size(); ++i)
      if(mFramebuffers[i] != VK_NULL_HANDLE)
        vkDestroyFramebuffer(device, mFramebuffers[i], nullptr);
}
