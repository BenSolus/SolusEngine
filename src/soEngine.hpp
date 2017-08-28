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

/**
 *  @file      soEngine.hpp
 *  @author    Bennet Carstensen
 *  @date      2017
 *  @copyright Copyright (c) 2017 Bennet Carstensen
 *
 *             Permission is hereby granted, free of charge, to any person
 *             obtaining a copy of this software and associated documentation
 *             files (the "Software"), to deal in the Software without
 *             restriction, including without limitation the rights to use,
 *             copy, modify, merge, publish, distribute, sublicense, and/or sell
 *             copies of the Software, and to permit persons to whom the
 *             Software is furnished to do so, subject to the following
 *             conditions:
 *
 *             The above copyright notice and this permission notice shall be
 *             included in all copies or substantial portions of the Software.
 *
 *             THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *             EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *             OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *             NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *             HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *             WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *             FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *             OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <vk/soVkCommandBuffers.hpp>
#include <vk/soVkDebugReportCallbackEXT.hpp>
#include <vk/soVkDepthImage.hpp>
#include <vk/soVkDescriptorPool.hpp>
#include <vk/soVkDescriptorSet.hpp>
#include <vk/soVkDescriptorSetLayout.hpp>
#include <vk/soVkFramebuffers.hpp>
#include <vk/soVkImageViews.hpp>
#include <vk/soVkIndexBuffer.hpp>
#include <vk/soVkMesh.hpp>
#include <vk/soVkPipeline.hpp>
#include <vk/soVkRenderPass.hpp>
#include <vk/soVkSemaphore.hpp>
#include <vk/soVkSurface.hpp>
#include <vk/soVkSwapChain.hpp>
#include <vk/soVkTextureSampler.hpp>
#include <vk/soVkUniformBuffer.hpp>
#include <vk/soVkVertexBuffer.hpp>

#include <map>

namespace so
{
  class
  Engine
  {
    public:
      Engine();

      void
      drawFrame();

      inline GLFWwindow* getWindow() { return mSurface.getGLFWwindow(); }

      inline VkDevice getVkDevice()
      { return mSwapChain.getDevice()->getVkDevice(); }

      void
      updateUniformBuffer();

    private:
      vk::CommandBuffers         mCommandBuffers;
      vk::DebugReportCallbackEXT mDebugCallback;
      vk::DepthImage             mDepthImage;
      vk::DescriptorPool         mDescriptorPool;
      vk::DescriptorSet          mDescriptorSet;
      vk::DescriptorSetLayout    mDescriptorSetLayout;
      vk::Framebuffers           mFramebuffers;
      vk::ImageViews             mDepthImageView;
      vk::ImageViews             mImageViews;
      vk::Mesh                   mModel;
      vk::Mesh                   mModel1;
      vk::Pipeline               mPipeline;
      vk::RenderPass             mRenderPass;
      vk::Semaphore              mImageAvailableSemaphore;
      vk::Semaphore              mRenderFinishedSemaphore;
      vk::Surface                mSurface;
      vk::SwapChain              mSwapChain;
      vk::TextureSampler         mTextureSampler;
      vk::UniformBuffer          mUniformBuffer;

      vk::SharedPtrLogicalDevice mDevice;
      vk::SharedPtrCommandPool   mCommandPool;

      std::size_t            mDynamicAlignment;

      void
      recreateSwapChain();

      static void
      onWindowResized(GLFWwindow* window, int width, int height);
  };
} //namespace so
