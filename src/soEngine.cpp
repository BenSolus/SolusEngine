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
#include <utils/err/soException.hpp>
#include <utils/fs/soFileSystem.hpp>

so::Engine::Engine()
  : mCommandBuffers(),
    mDebugCallback(),
    mDepthImage(),
    mDescriptorPool(),
    mDescriptorSetLayout(),
    mFramebuffers(),
    mDepthImageView(),
    mImageViews(),
    mPipeline(),
    mRenderPass(),
    mImageAvailableSemaphore(),
    mRenderFinishedSemaphore(),
    mSurface(),
    mSwapChain(),
    mTextureSampler(),
    mUniformBuffer(),
    mModels(),
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

    VkPhysicalDeviceProperties deviceProperties({});

    vkGetPhysicalDeviceProperties(mDevice->getVkPhysicalDevice(),
                                  &deviceProperties);

    std::size_t const uboAlignment
      (deviceProperties.limits.minUniformBufferOffsetAlignment);

    mDynamicAlignment        =
      (sizeof(vk::DynamicUBO) / uboAlignment) *
      uboAlignment +
      ((sizeof(vk::DynamicUBO) % uboAlignment) > 0 ? uboAlignment : 0);

    mUniformBuffer           = vk::UniformBuffer(mDevice, mDynamicAlignment);

    mUniformBuffer.updateViewProjectionUBO(mSwapChain.getVkExtent());

    mDescriptorPool          = vk::DescriptorPool(mDevice);

    mCommandBuffers          = vk::CommandBuffers(mDevice, mCommandPool);

    mImageAvailableSemaphore = vk::Semaphore(mDevice);
    mRenderFinishedSemaphore = vk::Semaphore(mDevice);
  }
  catch(...)
  {
    std::throw_with_nested(utils::err::Exception<std::runtime_error>
      ("failed to create the Solus Engine!", PRETTY_FUNCTION_SIG));
  }
}

void
so::Engine::drawFrame()
{
  if(mModels.size() not_eq 0)
  {
    uint32_t imageIndex;

    VkResult result(vkAcquireNextImageKHR
                      (getVkDevice(),
                       mSwapChain.getVkSwapchainKHR(),
                       std::numeric_limits<uint64_t>::max(),
                       mImageAvailableSemaphore.getVkSemaphore(),
                       VK_NULL_HANDLE,
                       &imageIndex));

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

    VkSemaphore waitSemaphores[]{mImageAvailableSemaphore.getVkSemaphore()};

    VkPipelineStageFlags waitStages[]
      { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores    = waitSemaphores;
    submitInfo.pWaitDstStageMask  = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    =
      &mCommandBuffers.getVkCommandBuffers()[imageIndex];

    VkSemaphore signalSemaphores[] =
      {mRenderFinishedSemaphore.getVkSemaphore()};

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = signalSemaphores;

    result = vkQueueSubmit(mDevice->getGraphicsVkQueue(),
                           1,
                           &submitInfo,
                           VK_NULL_HANDLE);


    if (result not_eq VK_SUCCESS)
      utils::err::Exception<std::runtime_error>("failed to submit draw "
                                                  "command buffer!",
                                                PRETTY_FUNCTION_SIG);

    VkPresentInfoKHR presentInfo = {};

    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {mSwapChain.getVkSwapchainKHR()};

    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    result = vkQueuePresentKHR(mDevice->getPresentVkQueue(), &presentInfo);

    if((result == VK_ERROR_OUT_OF_DATE_KHR) || (result == VK_SUBOPTIMAL_KHR))
      recreateSwapChain();
    else if(result != VK_SUCCESS)
      utils::err::Exception<std::runtime_error>("failed to present swap "
                                                "chain image!",
                                                PRETTY_FUNCTION_SIG);

    vkDeviceWaitIdle(mDevice->getVkDevice());
  }
}

void
so::Engine::recreateSwapChain()
{
  vkDeviceWaitIdle(mDevice->getVkDevice());

  mSwapChain = vk::SwapChain(mDevice,
                             mSurface.getVkSurfaceKHR(),
                             mSurface.getGLFWwindow(),
                             mSwapChain.getVkSwapchainKHR());

  mImageViews = vk::ImageViews(mDevice,
                               mSwapChain.getVkImages(),
                               mSwapChain.getVkFormat(),
                               VK_IMAGE_ASPECT_COLOR_BIT);

  mDepthImage = vk::DepthImage(mDevice, mCommandPool, mSwapChain);

  mDepthImageView = vk::ImageViews(mDevice,
                                   {mDepthImage.getResource()},
                                   mDepthImage.getVkFormat(),
                                   VK_IMAGE_ASPECT_DEPTH_BIT);

  mRenderPass = vk::RenderPass(mDevice, mSwapChain, mDepthImage);

  mPipeline = vk::Pipeline
    (mDevice,
     mSwapChain.getVkExtent(),
     mRenderPass.getVkRenderPass(),
     mDescriptorSetLayout.getVkDescriptorSetLayout());

  mFramebuffers = vk::Framebuffers(mDevice,
                                   mImageViews.getVkImageViews(),
                                   mRenderPass.getVkRenderPass(),
                                   mSwapChain.getVkExtent(),
                                   mDepthImageView.getVkImageViews()[0]);

  mCommandBuffers.recreateCommandBuffers(mFramebuffers,
                                         mPipeline,
                                         mRenderPass,
                                         mSwapChain,
                                         mUniformBuffer,
                                         mModels);
}

void
so::Engine::updateUniformBuffer()
{ mUniformBuffer.update(); }

void
so::Engine::addModel(std::string const& meshPath,
                     std::string const& texturePath,
                     std::size_t const  numInstances,
                     std::string const& meshKey,
                     std::string const& textureKey)
{
  std::string const realKey(meshKey.empty() ? std::to_string(mModels.size())
                                            : meshKey);

  try
  {
    /* Add texture. */

    std::string const realTextureKey(mTextureSampler.addTexture(texturePath,
                                                                textureKey));

    /* Add model. */
    auto const result
      (mModels.insert
        (std::pair<std::string, vk::Model>(realKey, vk::Model(meshPath,
                                                              realTextureKey,
                                                              numInstances))));

    /* 'result' is a pair of an iterator pointing to either the newly inserted
     * element or to the element with an equivalent key and a bool set to true
     * if a new element was inserted or false if an equivalent key already
     * existed. */

    if(not result.second)
      throw utils::err::Exception<std::runtime_error>("key already taken by a "
                                                      "model!",
                                                      PRETTY_FUNCTION_SIG);

    /* Get the overall number of dynamic uniform buffer objects needed. */

    std::size_t numDynamicUBOs(0);

    for(auto const& model : mModels)
      numDynamicUBOs += model.second.getNumInstances();

    /* Recreate dynamic uniform buffer object classes. */

    mUniformBuffer.recreateDynamicBuffer(numDynamicUBOs);

    numDynamicUBOs = 0;

    for(auto& model : mModels)
    {
      /* Set offset of dynamic uniform buffer objects for each model. */

      model.second.setDynamicUBOsOffset(numDynamicUBOs);

      numDynamicUBOs += model.second.getNumInstances();

      /* Recreate descriptor set for each model. */

      model.second.getDescriptorSet().recreateDescriptorSet
        (mDevice,
         mDescriptorPool,
         mDescriptorSetLayout,
         mTextureSampler,
         mUniformBuffer,
         model.second.getTextureKey(),
         model.second.getDynamicUBOsOffset(),
         model.second.getNumInstances());

      mCommandBuffers.recreateCommandBuffers(mFramebuffers,
                                             mPipeline,
                                             mRenderPass,
                                             mSwapChain,
                                             mUniformBuffer,
                                             mModels);
    }
  }
  catch(...)
  {
    std::throw_with_nested(utils::err::Exception<std::runtime_error>
                             ("failed to add Model \"" +
                              meshKey +
                              "\" (" +
                              meshPath +
                              ") with Texture \"" +
                              textureKey +
                              "\" (" +
                              texturePath +
                              ")",
                              PRETTY_FUNCTION_SIG));
  }
}

void
so::Engine::onWindowResized(GLFWwindow* window, int width, int height)
{
  if((width == 0) || (height == 0)) return;

  Engine* engine =
    reinterpret_cast<so::Engine*>(glfwGetWindowUserPointer(window));

  engine->recreateSwapChain();
}
