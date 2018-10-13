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

#include "soVkFramebuffers.hpp"

#include "cxx/soDebugCallback.hpp"

so::vk::Framebuffers::Framebuffers() noexcept
  : mFramebuffers(), mDevice(LogicalDevice::getSharedPtrNullDevice())
{}

so::vk::Framebuffers::~Framebuffers() noexcept { destroyMembers(); }

so::vk::Framebuffers&
so::vk::Framebuffers::operator=(Framebuffers &&other) noexcept
{
  if(this is_eq &other)
  {
    return *this;
  }

  destroyMembers();

  mFramebuffers = other.mFramebuffers;
  mDevice       = other.mDevice;

  other.mFramebuffers = std::vector<VkFramebuffer>();
  other.mDevice       = LogicalDevice::getSharedPtrNullDevice();

  return *this;
}

so::return_t
so::vk::Framebuffers::initialize(SharedPtrLogicalDevice const& device,
                                 SwapChain              const& swapChain,
                                 RenderPass             const& renderPass)
{
  mDevice = device;

  auto& swapChainImageViews{ swapChain.getImageViews().getVkImageViewsRef() };

  mFramebuffers.resize(swapChainImageViews.size());

  auto vkRenderPass{ renderPass.getVkRenderPass() };
  
  auto swapChainExtent{ swapChain.getVkExtent() };

  auto vkDevice{ mDevice->getVkDevice() };

  for(auto it{ swapChainImageViews.cbegin() };
           it != swapChainImageViews.cend();
           ++it)
  {
    VkImageView attachments[]{ *it };


    VkFramebufferCreateInfo framebufferInfo{};

    framebufferInfo.sType           =
      VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass      = vkRenderPass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments    = attachments;
    framebufferInfo.width           = swapChainExtent.width;
    framebufferInfo.height          = swapChainExtent.height;
    framebufferInfo.layers          = 1;
 
    auto idx{ std::distance(swapChainImageViews.begin(), it) };

    auto result{ vkCreateFramebuffer(vkDevice,
                                     &framebufferInfo,
                                     nullptr,
                                     &mFramebuffers[idx]) };

    if(result not_eq VK_SUCCESS)
    {
      DEBUG_CALLBACK(error,
                     "Failed to create a framebuffer.",
                     vkCreateFramebuffer);

      return failure;
    }
  }

  return success;
}

void
so::vk::Framebuffers::destroyMembers()
{
  VkDevice device(mDevice->getVkDevice());

  if(device != VK_NULL_HANDLE)
  {
    for(auto& framebuffer : mFramebuffers)
    {
      if(framebuffer != VK_NULL_HANDLE)
      {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
      }
    }
  }
}

