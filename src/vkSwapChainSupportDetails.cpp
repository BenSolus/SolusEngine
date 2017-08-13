/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vkSwapChainSupportDetails.hpp>

vk::eng::SwapChainSupportDetails::SwapChainSupportDetails()
  : mCapabilities({ }), mFormats({ }), mPresentModes({ })
{}

vk::eng::SwapChainSupportDetails::SwapChainSupportDetails
  (VkPhysicalDevice device, VkSurfaceKHR surface)
  : mCapabilities({ }), mFormats({ }), mPresentModes({ })
{
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR
    (device, surface, &mCapabilities);

  uint32_t formatCount;

  vkGetPhysicalDeviceSurfaceFormatsKHR(device,
                                       surface,
                                       &formatCount,
                                       nullptr);

  if(formatCount != 0)
  {
    mFormats.resize(formatCount);

    vkGetPhysicalDeviceSurfaceFormatsKHR(device,
                                         surface,
                                         &formatCount,
                                         mFormats.data());
  }

  uint32_t presentModeCount;

  vkGetPhysicalDeviceSurfacePresentModesKHR(device,
                                            surface,
                                            &presentModeCount,
                                            nullptr);

  if(presentModeCount != 0)
  {
    mPresentModes.resize(presentModeCount);

    vkGetPhysicalDeviceSurfacePresentModesKHR
    (
      device,
      surface,
      &presentModeCount,
      mPresentModes.data()
    );
  }
}
