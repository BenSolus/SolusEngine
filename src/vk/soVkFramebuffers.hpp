/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkFramebuffers.hpp
 *  @author    bennet carstensen
 *  @date      2017
 *  @copyright copyright (c) 2017 bennet carstensen
 *
 *             this source code form is subject to the terms of the
 *             mozilla public license, v. 2.0. if a copy of the mpl was
 *             not distributed with this file, you can obtain one at
 *             http://mozilla.org/mpl/2.0/.
 */

#pragma once

#include <vk/soVkLogicalDevice.hpp>

namespace so
{
  namespace vk
  {
    class
    Framebuffers
    {
      public:
        Framebuffers();

        Framebuffers(SharedPtrLogicalDevice const& device,
                     std::vector<VkImageView>&     swapChainImageViews,
                     VkRenderPass                  renderPass,
                     VkExtent2D                    swapChainExtent,
                     VkImageView                   depthImageView);

        Framebuffers(Framebuffers const& other) = delete;

        Framebuffers(Framebuffers&& other) = delete;

        ~Framebuffers() noexcept;

        Framebuffers&
        operator=(Framebuffers const& other) = delete;

        Framebuffers&
        operator=(Framebuffers&& other) noexcept
        {
          destroyMembers();
          
          mFramebuffers = other.mFramebuffers;
          mDevice       = other.mDevice;

          other.mFramebuffers = std::vector<VkFramebuffer>();
          other.mDevice       = SHARED_PTR_NULL_LOGICAL_DEVICE;

          return *this;
        }

        inline std::vector<VkFramebuffer>& getVkFramebuffers()
        { return mFramebuffers; }

      private:
        std::vector<VkFramebuffer> mFramebuffers;

        SharedPtrLogicalDevice     mDevice;

        void
        destroyMembers();
    };
  } // namespace vk
} // namespace so
