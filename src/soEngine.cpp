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

#include <soEngine.hpp>

#include <utils/def/soDefinitions.hpp>
#include <utils/fs/soFileSystem.hpp>

so::Engine::Engine()
  : mCommandBuffers(),
    mDebugCallback(),
    mDepthImage(),
    mDescriptorPool(),
    mDescriptorSet(),
    mDescriptorSetLayout(),
    mFramebuffers(),
    mDepthImageView(),
    mImageViews(),
    mModel(),
    mModel1(),
    mPipeline(),
    mRenderPass(),
    mImageAvailableSemaphore(),
    mRenderFinishedSemaphore(),
    mSurface(),
    mSwapChain(),
    mTextureSampler(),
    mUniformBuffer(),
    mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE),
    mCommandPool(SHARED_PTR_NULL_COMMAND_POOL),
    mDynamicAlignment(0)
{
  try
  {
    vk::SharedPtrInstance instance(std::make_shared<vk::Instance>());

    mDebugCallback       = vk::DebugReportCallbackEXT(instance);

    mSurface             = vk::Surface(mSurface.getGLFWwindow(), instance);

    glfwSetWindowUserPointer(mSurface.getGLFWwindow(), this);

    glfwSetWindowSizeCallback(mSurface.getGLFWwindow(),
                              Engine::onWindowResized);

    mDevice              = std::make_shared<vk::LogicalDevice>
                             (instance, mSurface.getVkSurfaceKHR());

    mSwapChain           = vk::SwapChain(mDevice,
                                         mSurface.getVkSurfaceKHR(),
                                         mSurface.getGLFWwindow());

    mImageViews          = vk::ImageViews(mDevice,
                                          mSwapChain.getVkImages(),
                                          mSwapChain.getVkFormat(),
                                          VK_IMAGE_ASPECT_COLOR_BIT);

    mDescriptorSetLayout = vk::DescriptorSetLayout(mDevice);

    mCommandPool         = std::make_shared<vk::CommandPool>(mDevice, mSurface);

    mDepthImage          = vk::DepthImage(mDevice, mCommandPool, mSwapChain);

    mDepthImageView      = vk::ImageViews(mDevice,
                                          { mDepthImage.getResource() },
                                          mDepthImage.getVkFormat(),
                                          VK_IMAGE_ASPECT_DEPTH_BIT);

    mRenderPass          = vk::RenderPass(mDevice, mSwapChain, mDepthImage);

    mPipeline            = vk::Pipeline
                             (mDevice,
                              mSwapChain.getVkExtent(),
                              mRenderPass.getVkRenderPass(),
                              mDescriptorSetLayout.getVkDescriptorSetLayout());

    mFramebuffers        = vk::Framebuffers
                             (mDevice,
                              mImageViews.getVkImageViews(),
                              mRenderPass.getVkRenderPass(),
                              mSwapChain.getVkExtent(),
                              mDepthImageView.getVkImageViews()[0]);

    mTextureSampler      = vk::TextureSampler(mDevice, mCommandPool);

    mTextureSampler.addTexture(BIN_DIR + "/data/textures/texture.png",
                               "body_0");

    mModel               = vk::Mesh(BIN_DIR + "/data/models/body.dae");

    VkPhysicalDeviceProperties deviceProperties;

    vkGetPhysicalDeviceProperties(mDevice->getVkPhysicalDevice(),
                                  &deviceProperties);

    const std::size_t uboAlignment
      (deviceProperties.limits.minUniformBufferOffsetAlignment);

    mDynamicAlignment    =
      (sizeof(vk::DynamicUBO) / uboAlignment) *
      uboAlignment +
      ((sizeof(vk::DynamicUBO) % uboAlignment) > 0 ? uboAlignment : 0);

    mUniformBuffer       = vk::UniformBuffer(mDevice, mDynamicAlignment, 2);

    mUniformBuffer.updateViewProjectionUBO(mSwapChain.getVkExtent());

    mDescriptorPool      = vk::DescriptorPool(mDevice);

    mDescriptorSet       = vk::DescriptorSet(mDevice,
                                             mDescriptorPool,
                                             mDescriptorSetLayout,
                                             mTextureSampler,
                                             mUniformBuffer,
                                             "body_0");

    mCommandBuffers      = vk::CommandBuffers(mDevice,
                                              mCommandPool,
                                              mFramebuffers.getVkFramebuffers(),
                                              mRenderPass.getVkRenderPass(),
                                              mDescriptorSet.getVkDescriptorSet(),
                                              mSwapChain.getVkExtent(),
                                              mPipeline.getVkPipeline(),
                                              mPipeline.getVkPipelineLayout(),
                                              mUniformBuffer);

    mImageAvailableSemaphore = vk::Semaphore(mDevice);
    mRenderFinishedSemaphore = vk::Semaphore(mDevice);
  }
  catch(...)
  {
    std::throw_with_nested(utils::err::Exception<std::runtime_error>
      ("failed to create Vulkan Framework!", PRETTY_FUNCTION_SIG));
  }
}

void
so::Engine::drawFrame()
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

  if(result is_eq VK_ERROR_OUT_OF_DATE_KHR)
  {
    recreateSwapChain();
    return;
  }
  else if((result not_eq VK_SUCCESS) and (result not_eq VK_SUBOPTIMAL_KHR))
    throw utils::err::Exception<std::runtime_error>("failed to acquire swap "
                                                    "chain image!",
                                                    PRETTY_FUNCTION_SIG);

  VkSubmitInfo submitInfo({});

  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[]{ mImageAvailableSemaphore.getVkSemaphore() };

  VkPipelineStageFlags waitStages[]
    { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

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

  result = vkQueueSubmit(mDevice->getGraphicsVkQueue(),
                         1,
                         &submitInfo,
                         VK_NULL_HANDLE);



  if(result not_eq VK_SUCCESS)
    utils::err::Exception<std::runtime_error>("failed to submit draw "
                                              "command buffer!",
                                              PRETTY_FUNCTION_SIG);

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
    utils::err::Exception<std::runtime_error>("failed to present swap "
                                              "chain image!",
                                              PRETTY_FUNCTION_SIG);

  vkDeviceWaitIdle(mDevice->getVkDevice());
}

void
so::Engine::recreateSwapChain()
{
  vkDeviceWaitIdle(mDevice->getVkDevice());

  mSwapChain      = vk::SwapChain(mDevice,
                                  mSurface.getVkSurfaceKHR(),
                                  mSurface.getGLFWwindow(),
                                  mSwapChain.getVkSwapchainKHR());

  mImageViews     = vk::ImageViews(mDevice,
                                   mSwapChain.getVkImages(),
                                   mSwapChain.getVkFormat(),
                                   VK_IMAGE_ASPECT_COLOR_BIT);

  mDepthImage     = vk::DepthImage(mDevice, mCommandPool, mSwapChain);

  mDepthImageView = vk::ImageViews(mDevice,
                                   { mDepthImage.getResource() },
                                    mDepthImage.getVkFormat(),
                                    VK_IMAGE_ASPECT_DEPTH_BIT);

  mRenderPass     = vk::RenderPass(mDevice, mSwapChain, mDepthImage);

  mPipeline       = vk::Pipeline
                      (mDevice,
                       mSwapChain.getVkExtent(),
                       mRenderPass.getVkRenderPass(),
                       mDescriptorSetLayout.getVkDescriptorSetLayout());

  mFramebuffers   = vk::Framebuffers(mDevice,
                                     mImageViews.getVkImageViews(),
                                     mRenderPass.getVkRenderPass(),
                                     mSwapChain.getVkExtent(),
                                     mDepthImageView.getVkImageViews()[0]);

  mCommandBuffers = vk::CommandBuffers
                      (mDevice,
                       mCommandPool,
                       mFramebuffers.getVkFramebuffers(),
                       mRenderPass.getVkRenderPass(),
                       mDescriptorSet.getVkDescriptorSet(),
                       mSwapChain.getVkExtent(),
                       mPipeline.getVkPipeline(),
                       mPipeline.getVkPipelineLayout(),
                       mUniformBuffer);
}

void
so::Engine::updateUniformBuffer()
{ mUniformBuffer.update(); }

void
so::Engine::onWindowResized(GLFWwindow* window, int width, int height)
{
  if((width == 0) || (height == 0)) return;

  Engine* engine =
    reinterpret_cast<so::Engine*>(glfwGetWindowUserPointer(window));

  engine->recreateSwapChain();
}
