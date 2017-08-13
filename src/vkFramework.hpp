/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file
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

#include <vkCommandBuffers.hpp>
#include <vkDebugReportCallbackEXT.hpp>
#include <vkDepthImage.hpp>
#include <vkDescriptorPool.hpp>
#include <vkDescriptorSet.hpp>
#include <vkDescriptorSetLayout.hpp>
#include <vkFramebuffers.hpp>
#include <vkImageViews.hpp>
#include <vkIndexBuffer.hpp>
#include <vkModel.hpp>
#include <vkPipeline.hpp>
#include <vkRenderPass.hpp>
#include <vkSemaphore.hpp>
#include <vkSurface.hpp>
#include <vkSwapChain.hpp>
#include <vkTextureImage.hpp>
#include <vkTextureSampler.hpp>
#include <vkUniformBuffer.hpp>

#include <vkVertexBuffer.hpp>

namespace vk
{
  namespace eng
  {
    class
    Framework
    {
      public:
        Framework();

        void
        drawFrame();

        inline GLFWwindow* getWindow() { return mSurface.getGLFWwindow(); }

        inline SharedPtrLogicalDevice getLogicalDevice()
        { return mSwapChain.getDevice()->shared_from_this(); }

        inline VkExtent2D getVkExtent2D() { return mSwapChain.getVkExtent(); }

        inline VkDevice getVkDevice()
        { return mSwapChain.getDevice()->getVkDevice(); }

        void
        updateUniformBuffer();

      private:
        CommandBuffers         mCommandBuffers;
        DebugReportCallbackEXT mDebugCallback;
        DepthImage             mDepthImage;
        DescriptorPool         mDescriptorPool;
        DescriptorSet          mDescriptorSet;
        DescriptorSetLayout    mDescriptorSetLayout;
        Framebuffers           mFramebuffers;
        ImageViews             mDepthImageView;
        ImageViews             mImageViews;
        ImageViews             mTextureImageView;
        IndexBuffer            mIndexBuffer;
        Model                  mModel;
        Pipeline               mPipeline;
        RenderPass             mRenderPass;
        Semaphore              mImageAvailableSemaphore;
        Semaphore              mRenderFinishedSemaphore;
        Surface                mSurface;
        SwapChain              mSwapChain;
        TextureImage           mTextureImage;
        TextureSampler         mTextureSampler;
        UniformBuffer          mUniformBuffer;
        VertexBuffer           mVertexBuffer;

        SharedPtrLogicalDevice mDevice;

        void
        recreateSwapChain();

        static void
        onWindowResized(GLFWwindow* window, int width, int height);
    };
  } // namespace vk
} //namespace fcm
