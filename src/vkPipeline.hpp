/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkPipeline.hpp
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

#include <vkLogicalDevice.hpp>

namespace vk
{
  namespace eng
  {
    class
    Pipeline
    {
      public:
        Pipeline();

        Pipeline(SharedPtrLogicalDevice const& device,
                 VkExtent2D                    swapChainExtent,
                 VkRenderPass                  renderPass,
                 VkDescriptorSetLayout&        descriptorSetLayout);

        Pipeline(Pipeline const& other) = delete;

        Pipeline(Pipeline&& other) = delete;

        ~Pipeline() noexcept;

        Pipeline&
        operator=(Pipeline const& other) = delete;

        Pipeline&
        operator=(Pipeline&& other) noexcept
        {
          destroyMembers();

          mPipeline       = other.mPipeline;
          mPipelineLayout = other.mPipelineLayout;
          mDevice         = other.mDevice;

          other.mPipeline       = VK_NULL_HANDLE;
          other.mPipelineLayout = VK_NULL_HANDLE;
          other.mDevice         = SHARED_PTR_NULL_LOGICAL_DEVICE;

          return *this;
        }

        inline VkPipeline getVkPipeline() { return mPipeline; }

        inline VkPipelineLayout getVkPipelineLayout()
        { return mPipelineLayout; }

      private:
        VkPipeline                mPipeline;
        VkPipelineLayout          mPipelineLayout;

        SharedPtrLogicalDevice    mDevice;

        void
        destroyMembers();
    };
  } // namespace eng
} // namespace vk
