/*
 * Copyright (C) 2017-2018 by Bennet Carstensen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <soVkSwapChainSupportDetails.hpp>

so::vk::SwapChainSupportDetails::SwapChainSupportDetails()
  : mCapabilities({ }), mFormats({ }), mPresentModes({ })
{}

so::vk::SwapChainSupportDetails::SwapChainSupportDetails
  (VkPhysicalDevice device, VkSurfaceKHR surface)
  : mCapabilities({ }), mFormats({ }), mPresentModes({ })
{
  getPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &mCapabilities);

  uint32_t formatCount;

  getPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

  if(formatCount not_eq 0)
  {
    mFormats.resize(formatCount);

    getPhysicalDeviceSurfaceFormatsKHR(device,
                                       surface,
                                       &formatCount,
                                       mFormats.data());
  }

  uint32_t presentModeCount;

  getPhysicalDeviceSurfacePresentModesKHR(device,
                                          surface,
                                          &presentModeCount,
                                          nullptr);

  if(presentModeCount not_eq 0)
  {
    mPresentModes.resize(presentModeCount);

    getPhysicalDeviceSurfacePresentModesKHR(device,
                                            surface,
                                            &presentModeCount,
                                            mPresentModes.data());
  }
}
