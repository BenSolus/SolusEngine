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

#include <vulkan/vulkan.h>

#include <vector>

namespace vk
{
  namespace eng
  {
    class
    SwapChainSupportDetails
    {
      public:
        SwapChainSupportDetails();

        SwapChainSupportDetails(VkPhysicalDevice device, VkSurfaceKHR surface);

        inline VkSurfaceCapabilitiesKHR*
        getCapabilitiesPointer() { return &mCapabilities; }

        inline VkSurfaceCapabilitiesKHR&
        getCapabilities() { return mCapabilities; }

        inline std::vector<VkSurfaceFormatKHR>& getFormats()
        { return mFormats; }

        inline void
        resizeFormats(std::vector<VkSurfaceFormatKHR>::size_type n)
        { mFormats.resize(n); }

        inline std::vector<VkPresentModeKHR>&
        getPresentModes() { return mPresentModes; }

        inline void
        resizePresentModes(std::vector<VkPresentModeKHR>::size_type n)
        { mPresentModes.resize(n); }

      private:
        VkSurfaceCapabilitiesKHR        mCapabilities;

        std::vector<VkSurfaceFormatKHR> mFormats;
        std::vector<VkPresentModeKHR>   mPresentModes;
    };
  } // namespace eng
} // namespace vk
