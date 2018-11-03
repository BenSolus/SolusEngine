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

/**
 *  @file      soVkSwapChain.hpp
 *  @author    Bennet Carstensen
 *  @date      2017
 *  @copyright Copyright (c) 2017-2018 Bennet Carstensen
 *
 *             Permission is hereby granted, free of charge, to any person
 *             obtaining a copy of this software and associated documentation
 *             files (the "Software"), to deal in the Software without
 *             restriction, including without limitation the rights to use,
 *             copy, modify, merge, publish, distribute, sublicense, and/or
 *             sell copies of the Software, and to permit persons to whom the
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

#include "soVkImageViews.hpp"
#include "soVkLogicalDevice.hpp"
#include "soVkSurface.hpp"

#include <vector>

namespace so {
namespace vk {
    
class
SwapChain
{
  public:
    SwapChain();

    SwapChain(SwapChain const& other) = delete;

    SwapChain(SwapChain&& other) = delete;

    ~SwapChain() noexcept;

    SwapChain&
    operator=(SwapChain const& other) = delete;

    SwapChain&
    operator=(SwapChain&& other) noexcept;

    return_t
    initialize(SharedPtrLogicalDevice const& device,
               Surface                const& surface);

    return_t
    reset(Surface const& surface);

    inline VkSwapchainKHR getVkSwapchainKHR() { return mSwapChain; }

    inline VkExtent2D getVkExtent() const { return mSwapChainExtent; }

    inline VkFormat getVkFormat() const { return mSwapChainImageFormat; }

    inline std::vector<VkImage> const&
    getVkImages() const { return mSwapChainImages; }

    inline ImageViews const&
    getImageViews() const { return mSwapChainImageViews; }

    inline SharedPtrLogicalDevice
    getDevice() { return mDevice->shared_from_this(); }

  private:
    VkSwapchainKHR         mSwapChain;

    VkExtent2D             mSwapChainExtent;
    VkFormat               mSwapChainImageFormat;

    std::vector<VkImage>   mSwapChainImages;
    ImageViews             mSwapChainImageViews;
   

    SharedPtrLogicalDevice mDevice;

    VkSurfaceFormatKHR
    chooseSwapSurfaceFormat
      (std::vector<VkSurfaceFormatKHR> const& availableFormats);

    VkPresentModeKHR
    chooseSwapPresentMode
      (std::vector<VkPresentModeKHR> const& availablePresentModes);

    VkExtent2D
    chooseSwapExtent(VkSurfaceCapabilitiesKHR const& capabilities,
                     Surface                  const& surface);

    return_t
    initializeMembers(Surface const& surface);

    void
    destroyMembers();
};

} // namespace vk
} // namespace so

