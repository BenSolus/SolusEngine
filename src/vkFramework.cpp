/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vkFramework.hpp>
#include <cc/ccConstants.hpp>


#include <iostream>

vk::eng::Framework::Framework()
  : mCommandBuffers(),
    mDebugCallback(),
    mDepthImage(),
    mDescriptorPool(),
    mDescriptorSet(),
    mDescriptorSetLayout(),
    mFramebuffers(),
    mDepthImageView(),
    mImageViews(),
    mTextureImageView(),
    mIndexBuffer(),
    mModel(),
    mPipeline(),
    mRenderPass(),
    mImageAvailableSemaphore(),
    mRenderFinishedSemaphore(),
    mSurface(),
    mSwapChain(),
    mTextureImage(),
    mTextureSampler(),
    mUniformBuffer(),
    mVertexBuffer(),
    mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE)
{
  try
  {
    SharedPtrInstance instance(std::make_shared<Instance>());

    mDebugCallback = vk::eng::DebugReportCallbackEXT(instance);

    mSurface       = vk::eng::Surface(mSurface.getGLFWwindow(), instance);

    glfwSetWindowUserPointer(mSurface.getGLFWwindow(), this);

    glfwSetWindowSizeCallback(mSurface.getGLFWwindow(),
                              Framework::onWindowResized);

    mDevice        = std::make_shared<LogicalDevice>
                       (instance, mSurface.getVkSurfaceKHR());

    mSwapChain     = vk::eng::SwapChain(mDevice,
                                        mSurface.getVkSurfaceKHR(),
                                        mSurface.getGLFWwindow());

    mImageViews    = vk::eng::ImageViews(mDevice,
                                         mSwapChain.getVkImages(),
                                         mSwapChain.getVkFormat(),
                                         VK_IMAGE_ASPECT_COLOR_BIT);

    mDescriptorSetLayout = vk::eng::DescriptorSetLayout(mDevice);



    SharedPtrCommandPool commandPool
      (std::make_shared<CommandPool>(mDevice, mSurface.getVkSurfaceKHR()));

    mDepthImage       = vk::eng::DepthImage(mDevice,
                                            mSwapChain.getVkExtent(),
                                            commandPool->getVkCommandPool());
    mDepthImageView   = vk::eng::ImageViews(mDevice,
                                            { mDepthImage.getResource() },
                                            mDepthImage.getVkFormat(),
                                            VK_IMAGE_ASPECT_DEPTH_BIT);

    mRenderPass       = vk::eng::RenderPass(mDevice,
                                            mSwapChain.getVkFormat(),
                                            mDepthImage.getVkFormat());

    mPipeline         = vk::eng::Pipeline
                          (mDevice,
                           mSwapChain.getVkExtent(),
                           mRenderPass.getVkRenderPass(),
                           mDescriptorSetLayout.getVkDescriptorSetLayout());

    mFramebuffers  = vk::eng::Framebuffers(mDevice,
                                           mImageViews.getVkImageViews(),
                                           mRenderPass.getVkRenderPass(),
                                           mSwapChain.getVkExtent(),
                                           mDepthImageView.getVkImageViews()[0]);

    mTextureImage     = vk::eng::TextureImage
                          (mDevice,
                           BIN_DIR + "/data/textures/texture.png",
                           commandPool->getVkCommandPool());

    mTextureSampler   = vk::eng::TextureSampler(mDevice);

    mTextureImageView = vk::eng::ImageViews(mDevice,
                                            { mTextureImage.getResource() },
                                            VK_FORMAT_R8G8B8A8_UNORM,
                                            VK_IMAGE_ASPECT_COLOR_BIT);

    mModel          = vk::eng::Model(BIN_DIR + "/data/models/body.glb");

    mVertexBuffer   = vk::eng::VertexBuffer(mDevice,
                                            commandPool->getVkCommandPool());

    mIndexBuffer    = vk::eng::IndexBuffer(mDevice,
                                           commandPool->getVkCommandPool());

    mUniformBuffer  = vk::eng::UniformBuffer(mDevice);

    mDescriptorPool = vk::eng::DescriptorPool(mDevice);

    mDescriptorSet  = vk::eng::DescriptorSet
                        (mDescriptorSetLayout.getVkDescriptorSetLayout(),
                         mDescriptorPool.getVkDescriptorPool(),
                         mDevice->getVkDevice(),
                         mUniformBuffer.getResource(),
                         mTextureImageView.getVkImageViews()[0],
                         mTextureSampler.getVkSampler());

    mCommandBuffers = vk::eng::CommandBuffers
                        (mDevice,
                         commandPool,
                         mFramebuffers.getVkFramebuffers(),
                         mRenderPass.getVkRenderPass(),
                         mDescriptorSet.getVkDescriptorSet(),
                         mSwapChain.getVkExtent(),
                         mPipeline.getVkPipeline(),
                         mPipeline.getVkPipelineLayout(),
                         mVertexBuffer.getResource(),
                         mIndexBuffer.getResource());

    mImageAvailableSemaphore = vk::eng::Semaphore(mDevice);
    mRenderFinishedSemaphore = vk::eng::Semaphore(mDevice);
  }
  catch(...)
  {
    cc::throw_with_nested<std::runtime_error>("failed to create Vulkan "
                                              "Framework!",
                                              PRETTY_FUNCTION_SIG);
  }
}

void
vk::eng::Framework::drawFrame()
{
  uint32_t imageIndex;

  VkResult result(vkAcquireNextImageKHR
                  (
                    getVkDevice(),
                    mSwapChain.getVkSwapchainKHR(),
                    std::numeric_limits<uint64_t>::max(),
                    mImageAvailableSemaphore.getVkSemaphore(),
                    VK_NULL_HANDLE,
                    &imageIndex
                  ));

  if(result == VK_ERROR_OUT_OF_DATE_KHR)
  {
    recreateSwapChain();
    return;
  }
  else if((result != VK_SUCCESS) && (result != VK_SUBOPTIMAL_KHR))
    cc::throw_with_nested<std::runtime_error>("failed to acquire swap "
                                              "chain image!",
                                              PRETTY_FUNCTION_SIG);

  VkSubmitInfo submitInfo = {};

  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = { mImageAvailableSemaphore.getVkSemaphore() };

  VkPipelineStageFlags waitStages[] =
    {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores    = waitSemaphores;
  submitInfo.pWaitDstStageMask  = waitStages;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers    =
    &mCommandBuffers.getVkCommandBuffers()[imageIndex];

  VkSemaphore signalSemaphores[] =
    { mRenderFinishedSemaphore.getVkSemaphore() };

  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores    = signalSemaphores;

  if(vkQueueSubmit(mDevice->getGraphicsVkQueue(), 1, &submitInfo, VK_NULL_HANDLE)
     != VK_SUCCESS)
  {
    cc::throw_with_nested<std::runtime_error>("failed to submit draw "
                                               "command buffer!", PRETTY_FUNCTION_SIG);
  }

  VkPresentInfoKHR presentInfo = {};

  presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores    = signalSemaphores;

  VkSwapchainKHR swapChains[] = { mSwapChain.getVkSwapchainKHR() };

  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains    = swapChains;
  presentInfo.pImageIndices  = &imageIndex;
  presentInfo.pResults       = nullptr;

  result = vkQueuePresentKHR(mDevice->getPresentVkQueue(), &presentInfo);

  if((result == VK_ERROR_OUT_OF_DATE_KHR) || (result == VK_SUBOPTIMAL_KHR))
    recreateSwapChain();
  else if(result != VK_SUCCESS)
    cc::throw_with_nested<std::runtime_error>("failed to present swap "
                                              "chain image!",
                                              PRETTY_FUNCTION_SIG);

  vkDeviceWaitIdle(mDevice->getVkDevice());
}

void
vk::eng::Framework::recreateSwapChain()
{
  vkDeviceWaitIdle(mDevice->getVkDevice());

  mSwapChain      = vk::eng::SwapChain(mDevice,
                                       mSurface.getVkSurfaceKHR(),
                                       mSurface.getGLFWwindow(),
                                       mSwapChain.getVkSwapchainKHR());

  mImageViews     = vk::eng::ImageViews(mDevice,
                                        mSwapChain.getVkImages(),
                                        mSwapChain.getVkFormat(),
                                        VK_IMAGE_ASPECT_COLOR_BIT);

  mDepthImage     = vk::eng::DepthImage(mDevice,
                                        mSwapChain.getVkExtent(),
                                        mCommandBuffers.getCommandPool()->getVkCommandPool());

  mDepthImageView = vk::eng::ImageViews(mDevice,
                                        { mDepthImage.getResource() },
                                        mDepthImage.getVkFormat(),
                                        VK_IMAGE_ASPECT_DEPTH_BIT);

  mRenderPass     = vk::eng::RenderPass(mDevice,
                                        mSwapChain.getVkFormat(),
                                        mDepthImage.getVkFormat());

  mPipeline       = vk::eng::Pipeline
                      (mDevice,
                       mSwapChain.getVkExtent(),
                       mRenderPass.getVkRenderPass(),
                       mDescriptorSetLayout.getVkDescriptorSetLayout());

  mFramebuffers   = vk::eng::Framebuffers(mDevice,
                                          mImageViews.getVkImageViews(),
                                          mRenderPass.getVkRenderPass(),
                                          mSwapChain.getVkExtent(),
                                          mDepthImageView.getVkImageViews()[0]);

  mCommandBuffers = vk::eng::CommandBuffers
                      (mDevice,
                       mCommandBuffers.getCommandPool(),
                       mFramebuffers.getVkFramebuffers(),
                       mRenderPass.getVkRenderPass(),
                       mDescriptorSet.getVkDescriptorSet(),
                       mSwapChain.getVkExtent(),
                       mPipeline.getVkPipeline(),
                       mPipeline.getVkPipelineLayout(),
                       mVertexBuffer.getResource(),
                       mIndexBuffer.getResource());
}

void
vk::eng::Framework::updateUniformBuffer()
{ mUniformBuffer.update(mSwapChain.getVkExtent()); }

void
vk::eng::Framework::onWindowResized(GLFWwindow* window, int width, int height)
{
  if((width == 0) || (height == 0)) return;

  Framework* framework =
    reinterpret_cast<vk::eng::Framework*>(glfwGetWindowUserPointer(window));

  framework->recreateSwapChain();
}
