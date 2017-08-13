/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkSwapChain.hpp
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

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

namespace vk
{
  namespace eng
  {
    class
    SwapChain
    {
      public:
        SwapChain();

        SwapChain(SharedPtrLogicalDevice const& device,
                  VkSurfaceKHR                  surface,
                  GLFWwindow*                   window,
                  VkSwapchainKHR                oldSwapChain = VK_NULL_HANDLE);

        SwapChain(SwapChain const& other) = delete;

        SwapChain(SwapChain&& other) = delete;

        ~SwapChain() noexcept;

        SwapChain&
        operator=(SwapChain const& other) = delete;

        SwapChain&
        operator=(SwapChain&& other) noexcept
        {
          destroyMembers();

          mSwapChain            = other.mSwapChain;
          mSwapChainExtent      = other.mSwapChainExtent;
          mSwapChainImageFormat = other.mSwapChainImageFormat;
          mSwapChainImages      = other.mSwapChainImages;
          mDevice               = other.mDevice;

          other.mSwapChain            = VK_NULL_HANDLE;
          other.mSwapChainExtent      = { 0, 0 };
          other.mSwapChainImageFormat = VK_FORMAT_UNDEFINED;
          other.mSwapChainImages      = std::vector<VkImage>();
          other.mDevice               = SHARED_PTR_NULL_LOGICAL_DEVICE;

          return *this;
        }

        inline VkSwapchainKHR getVkSwapchainKHR() { return mSwapChain; }

        inline VkExtent2D getVkExtent() { return mSwapChainExtent; }

        inline VkFormat getVkFormat() { return mSwapChainImageFormat; }

        inline std::vector<VkImage>& getVkImages()
        { return mSwapChainImages; }

        inline SharedPtrLogicalDevice getDevice()
        { return mDevice->shared_from_this(); }

      private:
        VkSwapchainKHR         mSwapChain;

        VkExtent2D             mSwapChainExtent;
        VkFormat               mSwapChainImageFormat;

        std::vector<VkImage>   mSwapChainImages;

        SharedPtrLogicalDevice mDevice;

        VkSurfaceFormatKHR
        chooseSwapSurfaceFormat
          (const std::vector<VkSurfaceFormatKHR>& availableFormats);

        VkPresentModeKHR
        chooseSwapPresentMode
          (const std::vector<VkPresentModeKHR> availablePresentModes);

        VkExtent2D
        chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities,
                         GLFWwindow*                     window);

        void
        destroyMembers();
    };
  } // namespace eng
} // namespace vk
