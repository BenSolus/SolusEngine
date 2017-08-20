/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vkFramework.hpp>

#include <cc/ccConstants.hpp>

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
    mModel1(),
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
    mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE),
    mCommandPool(SHARED_PTR_NULL_COMMAND_POOL),
    mDynamicAlignment(0)
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

    mDescriptorSetLayout = DescriptorSetLayout(mDevice);

    mCommandPool         = std::make_shared<CommandPool>(mDevice, mSurface);

    mDepthImage          = DepthImage(mDevice, mCommandPool, mSwapChain);

    mDepthImageView      = ImageViews(mDevice,
                                      { mDepthImage.getResource() },
                                      mDepthImage.getVkFormat(),
                                      VK_IMAGE_ASPECT_DEPTH_BIT);

    mRenderPass          = RenderPass(mDevice, mSwapChain, mDepthImage);

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

    mTextureImage     = TextureImage(mDevice,
                                     mCommandPool,
                                     BIN_DIR + "/data/textures/texture.png");

    mTextureSampler   = vk::eng::TextureSampler(mDevice);

    mTextureImageView = vk::eng::ImageViews(mDevice,
                                            { mTextureImage.getResource() },
                                            VK_FORMAT_R8G8B8A8_UNORM,
                                            VK_IMAGE_ASPECT_COLOR_BIT);

    mModel          = vk::eng::Model(BIN_DIR + "/data/models/body.dae");

    mVertexBuffer   = VertexBuffer(mDevice, mCommandPool);

    mIndexBuffer    = IndexBuffer(mDevice, mCommandPool);

    VkPhysicalDeviceProperties deviceProperties;

    vkGetPhysicalDeviceProperties(mDevice->getVkPhysicalDevice(),
                                  &deviceProperties);

    const std::size_t uboAlignment
      (deviceProperties.limits.minUniformBufferOffsetAlignment);

    mDynamicAlignment =
      (sizeof(DynamicUBO) / uboAlignment) *
      uboAlignment +
      ((sizeof(DynamicUBO) % uboAlignment) > 0 ? uboAlignment : 0);

    mUniformBuffer  = UniformBuffer(mDevice, mDynamicAlignment, 125);

    mUniformBuffer.updateViewProjectionUBO(mSwapChain.getVkExtent());

    mDescriptorPool = vk::eng::DescriptorPool(mDevice);

    mDescriptorSet  = vk::eng::DescriptorSet
                        (mDescriptorSetLayout.getVkDescriptorSetLayout(),
                         mDescriptorPool.getVkDescriptorPool(),
                         mDevice->getVkDevice(),
                         mTextureImageView.getVkImageViews()[0],
                         mTextureSampler.getVkSampler(),
                         mUniformBuffer);

    mCommandBuffers          = CommandBuffers(mDevice,
                                              mCommandPool,
                                              mFramebuffers.getVkFramebuffers(),
                                              mRenderPass.getVkRenderPass(),
                                              mDescriptorSet.getVkDescriptorSet(),
                                              mSwapChain.getVkExtent(),
                                              mPipeline.getVkPipeline(),
                                              mPipeline.getVkPipelineLayout(),
                                     mVertexBuffer.getResource(),
                                     mIndexBuffer.getResource(),
                                     mUniformBuffer);

    mImageAvailableSemaphore = Semaphore(mDevice);
    mRenderFinishedSemaphore = Semaphore(mDevice);
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

  mDepthImage     = DepthImage(mDevice, mCommandPool, mSwapChain);

  mDepthImageView = vk::eng::ImageViews(mDevice,
                                        { mDepthImage.getResource() },
                                        mDepthImage.getVkFormat(),
                                        VK_IMAGE_ASPECT_DEPTH_BIT);

  mRenderPass     = RenderPass(mDevice, mSwapChain, mDepthImage);

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
                       mCommandPool,
                       mFramebuffers.getVkFramebuffers(),
                       mRenderPass.getVkRenderPass(),
                       mDescriptorSet.getVkDescriptorSet(),
                       mSwapChain.getVkExtent(),
                       mPipeline.getVkPipeline(),
                       mPipeline.getVkPipelineLayout(),
                       mVertexBuffer.getResource(),
                       mIndexBuffer.getResource(),
                       mUniformBuffer);
}

void
vk::eng::Framework::updateUniformBuffer()
{ mUniformBuffer.update(); }

void
vk::eng::Framework::onWindowResized(GLFWwindow* window, int width, int height)
{
  if((width == 0) || (height == 0)) return;

  Framework* framework =
    reinterpret_cast<vk::eng::Framework*>(glfwGetWindowUserPointer(window));

  framework->recreateSwapChain();
}
