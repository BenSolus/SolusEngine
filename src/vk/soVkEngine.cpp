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

#include "soVkEngine.hpp"

#include "cxx/soDebugCallback.hpp"


so::Engine::Engine()
  : mDebugCallback(),
    mSurface(), 
    mSwapChain(),
    mRenderPass(),
    mPipeline(),
    mFramebuffers(),
    mCommandBuffers(),
    mImageAvailableSemaphores(),
    mRenderFinishedSemaphores(),
    mInFlightFences(),
    mCurrentFrame(0)
{}

so::Engine::~Engine() noexcept
{
  VkDevice device{ getVkDevice() };

  if(device not_eq VK_NULL_HANDLE)
  {
    vkDeviceWaitIdle(device);
  }
}


so::return_t
so::Engine::initialize(std::string const& applicationName,
                       uint32_t    const  applicationVersion,
                       size_type   const  maxFramesInFlight)
{
  so::return_t result;

  if(mSurface.initialize() is_eq failure)
  {
    return failure;
  }
 
  vk::SharedPtrInstance instance{ std::make_shared<vk::Instance>() };

  std::vector<char const*> instanceExtensions;

  if(mSurface.getInstanceExtensions(instanceExtensions) is_eq failure)
  {
    return failure;
  }

  result = instance->initialize(applicationName,
                                applicationVersion,
                                instanceExtensions);
 
  if(result is_eq failure)
  {
    return failure;
  }

  if(mDebugCallback.initialize(instance) is_eq failure)
  {
    return failure;
  }
  
  mSurface.setSharedPtrInstance(instance);

  if(mSurface.createWindow(applicationName) is_eq failure)
  {
    return failure;
  }
  
  if(mSurface.createSurface() is_eq failure)
  {
    return failure;
  }

  vk::SharedPtrLogicalDevice device{ std::make_shared<vk::LogicalDevice>() };

  if(device->initialize(instance, mSurface) is_eq failure)
  {
    return failure;
  }

  if(mSwapChain.initialize(device, mSurface) is_eq failure)
  {
    return failure;
  }

  vk::SharedPtrCommandPool commandPool{ std::make_shared<vk::CommandPool>() };

  if(commandPool->initialize(device, mSurface) is_eq failure)
  {
    std::string message{ "Failed to create a command pool." };

    DEBUG_CALLBACK(error, message, vk::CommandPool::initialize);

    return failure;
  }

  if(mRenderPass.initialize(device, mSwapChain) is_eq failure)
  {
    std::string message{ "Failed to create a render pass." };
 
    DEBUG_CALLBACK(error, message, vk::RenderPass::initialize);

    return failure;
  }

  if(mPipeline.initialize(device, mSwapChain, mRenderPass) is_eq failure)
  {
    std::string message{ "Failed to create a graphics pipeline." };

    DEBUG_CALLBACK(error, message, vk::Pipeline::initialize);

    return failure;
  }

  if(mFramebuffers.initialize(device, mSwapChain, mRenderPass) is_eq failure)
  {
    std::string message{ "Failed to create framebuffers." };

    DEBUG_CALLBACK(error, message, vk::Framebuffers::initialize);

    return failure;
  }

  result = mCommandBuffers.initialize(device,
                                      commandPool,
                                      mFramebuffers,
                                      mRenderPass,
                                      mSwapChain,
                                      mPipeline);
  
  if(result is_eq failure)
  {
    std::string message{ "Failed to create command buffers. " };

    DEBUG_CALLBACK(error, message, vk::CommandBuffers::initialize);

    return failure;
  }

  result = mImageAvailableSemaphores.initialize(device, maxFramesInFlight);
  result = result is_eq failure
             ? failure
             : mRenderFinishedSemaphores.initialize(device, maxFramesInFlight);

  if(result is_eq failure)
  {
    DEBUG_CALLBACK(error,
                   "Failed to create a semaphore.",
                   vk::Semaphores::initialize);

    return failure;
  }

  if(mInFlightFences.initialize(device, maxFramesInFlight) is_eq failure)
  {
    DEBUG_CALLBACK(error,
                   "Failed to create a fence for in-flight frames.",
                   vk::Fences<>::initialize);

    return failure;
  }

  return success;
}

so::return_t
so::Engine::drawFrame()
{
  VkDevice device{ mSwapChain.getDevice()->getVkDevice() };

  vkWaitForFences(device,
                  1,
                  &mInFlightFences[mCurrentFrame],
                  VK_TRUE,
                  std::numeric_limits<uint64_t>::max());

  vkResetFences(device, 1, &mInFlightFences[mCurrentFrame]);

  uint32_t imageIndex;

  VkSemaphore imageAvailableSemaphore
    { mImageAvailableSemaphores.getVkSemaphoresRef()[mCurrentFrame] };

  VkSemaphore renderFinishedSemaphore
    { mRenderFinishedSemaphores.getVkSemaphoresRef()[mCurrentFrame] };

  vkAcquireNextImageKHR(getVkDevice(),
                        mSwapChain.getVkSwapchainKHR(),
                        std::numeric_limits<uint64_t>::max(),
                        imageAvailableSemaphore,
                        VK_NULL_HANDLE,
                        &imageIndex);

  VkSubmitInfo submitInfo{};

  submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[]{ imageAvailableSemaphore };

  VkPipelineStageFlags waitStages[]
    { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

  submitInfo.waitSemaphoreCount   = 1;
  submitInfo.pWaitSemaphores      = waitSemaphores;
  submitInfo.pWaitDstStageMask    = waitStages;
  submitInfo.commandBufferCount   = 1;
  submitInfo.pCommandBuffers      =
    &mCommandBuffers.getVkCommandBuffersRef()[imageIndex];

  VkSemaphore signalSemaphores[]{ renderFinishedSemaphore };

  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores    = signalSemaphores;

  VkResult result{ vkQueueSubmit(mSwapChain.getDevice()->getGraphicsVkQueue(),
                                 1,
                                 &submitInfo,
                                 mInFlightFences[mCurrentFrame]) };
  
  if(result not_eq VK_SUCCESS)
  {
    DEBUG_CALLBACK(error,
                   "Failed to submit draw command buffer.",
                   vkQueueSubmit);
  
    return failure;
  }
  
  VkPresentInfoKHR presentInfo{};

  presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores    = signalSemaphores;

  VkSwapchainKHR swapChains[]{ mSwapChain.getVkSwapchainKHR() };

  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains    = swapChains;
  presentInfo.pImageIndices  = &imageIndex;
  presentInfo.pResults       = nullptr; // optional

  vkQueuePresentKHR(mSwapChain.getDevice()->getPresentVkQueue(), &presentInfo);

  vkQueueWaitIdle(mSwapChain.getDevice()->getPresentVkQueue());

  size_type const maxFramesInFlight
    { mImageAvailableSemaphores.getVkSemaphoresRef().size() };

  mCurrentFrame = (mCurrentFrame + 1) %
                  static_cast<index_t>(maxFramesInFlight); 

  return success;
}

