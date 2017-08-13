/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkRenderPass.hpp
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
    RenderPass
    {
      public:
        RenderPass();

        RenderPass(SharedPtrLogicalDevice const& device,
                   VkFormat                      swapChainImageFormat,
                   VkFormat                      depthFormat);

        RenderPass(RenderPass const& other) = delete;

        RenderPass(RenderPass&& other) = delete;

        ~RenderPass() noexcept;

        RenderPass&
        operator=(RenderPass const& other) = delete;

        RenderPass&
        operator=(RenderPass&& other) noexcept
        {
          destroyMembers();
          
          mRenderPass = other.mRenderPass;
          mDevice     = other.mDevice;

          other.mRenderPass = VK_NULL_HANDLE;
          other.mDevice     = SHARED_PTR_NULL_LOGICAL_DEVICE;

          return *this;
        }

        inline VkRenderPass getVkRenderPass() { return mRenderPass; }

      private:
        VkRenderPass           mRenderPass;

        SharedPtrLogicalDevice mDevice;

        void
        destroyMembers();
    };
  } // namespace eng
} // namespace vk
