/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      soEngine.hpp
 *  @author    Bennet Carstensen
 *  @date      2017
 *  @copyright Copyright (c) 2017 Bennet Carstensen
 *
 *             This Source Code Form is subject to the terms of the
 *             Mozilla Public License, v. 2.0. If a copy of the MPL was
 *             not distributed with this file, You can obtain one at
 *             http://mozilla.org/MPL/2.0/.
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
