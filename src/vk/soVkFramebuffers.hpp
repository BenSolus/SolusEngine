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
 *  @file      soVkFramebuffers.hpp
 *  @author    bennet carstensen
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
