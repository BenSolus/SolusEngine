/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vkFramebuffers.hpp>

#include <ccException.hpp>

vk::eng::Framebuffers::Framebuffers()
  : mFramebuffers(), mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE)
{}

vk::eng::Framebuffers::Framebuffers
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

    if(vkCreateFramebuffer(device->getVkDevice(), &framebufferInfo, nullptr, &mFramebuffers[i]) != VK_SUCCESS)
    {
      cc::throw_with_nested<std::runtime_error>("failed to create "
                                                "framebuffer!", PRETTY_FUNCTION_SIG);
    }
  }
}

vk::eng::Framebuffers::~Framebuffers() noexcept { destroyMembers(); }

void
vk::eng::Framebuffers::destroyMembers()
{
  VkDevice device(mDevice->getVkDevice());

  if(device != VK_NULL_HANDLE)
    for(size_t i(0); i < mFramebuffers.size(); ++i)
      if(mFramebuffers[i] != VK_NULL_HANDLE)
        vkDestroyFramebuffer(device, mFramebuffers[i], nullptr);
}
