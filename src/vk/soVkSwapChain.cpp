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

#include "soVkSwapChain.hpp"

#include "soVkQueueFamilyIndices.hpp"
#include "soVkSwapChainSupportDetails.hpp"

#include "cxx/soDebugCallback.hpp"
#include "cxx/soDefinitions.hpp"

so::vk::SwapChain::SwapChain()
  : mSwapChain(VK_NULL_HANDLE),
    mSwapChainExtent({ 0, 0 }),
    mSwapChainImageFormat(VK_FORMAT_UNDEFINED),
    mSwapChainImages(),
    mSwapChainImageViews(),
    mDevice(LogicalDevice::getSharedPtrNullDevice())
{}

so::return_t
so::vk::SwapChain::initialize(SharedPtrLogicalDevice const& device,
                              Surface const&                surface, 
                              VkSwapchainKHR                oldSwapChain)
{
  mDevice = device;

  VkDevice         vkDevice{ device->getVkDevice() };
  VkPhysicalDevice physicalDevice{ device->getVkPhysicalDevice() };

  SwapChainSupportDetails swapChainSupport{ physicalDevice,
                                            surface.getVkSurfaceKHR()};

  VkSurfaceFormatKHR surfaceFormat
    { chooseSwapSurfaceFormat(swapChainSupport.getFormats()) };
  VkPresentModeKHR presentMode
    { chooseSwapPresentMode(swapChainSupport.getPresentModes()) };
  VkExtent2D extent{ chooseSwapExtent(swapChainSupport.getCapabilities(),
                                      surface) };

  uint32_t imageCount
    { swapChainSupport.getCapabilities().minImageCount + 1 };

  uint32_t const maxImageCount
    { swapChainSupport.getCapabilities().maxImageCount };
  
  bool const imageLimitDefined{ maxImageCount > 0 };
  bool const maxSmallerMin{ imageCount > maxImageCount };

  if(imageLimitDefined and maxSmallerMin)
  {
    imageCount = maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo{};

  createInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface          = surface.getVkSurfaceKHR();
  createInfo.minImageCount    = imageCount;
  createInfo.imageFormat      = surfaceFormat.format;
  createInfo.imageColorSpace  = surfaceFormat.colorSpace;
  createInfo.imageExtent      = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  QueueFamilyIndices indices{ physicalDevice, surface };

  uint32_t queueFamilyIndices[]
    { static_cast<uint32_t>(indices.getGraphicsFamily()),
      static_cast<uint32_t>(indices.getPresentFamily()) };

  if(indices.getGraphicsFamily() not_eq indices.getPresentFamily())
  {
    createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices   = queueFamilyIndices;
  }
  else
  {   
    createInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
  }

  createInfo.preTransform   =
    swapChainSupport.getCapabilities().currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode    = presentMode;
  createInfo.clipped        = VK_TRUE;
  createInfo.oldSwapchain   = oldSwapChain;

  VkResult const swapChainResult(vkCreateSwapchainKHR(vkDevice,
                                                      &createInfo,
                                                      nullptr,
                                                      &mSwapChain));

  if(swapChainResult not_eq VK_SUCCESS)
  {
    std::string message{ ": Failed to create swap chain." };

    PREPEND_FUNCTION_SIG_TO_STRING(message);

    executeDebugCallback(error, message);

    return failure;  
  }

  vkGetSwapchainImagesKHR(vkDevice, mSwapChain, &imageCount, nullptr);
  
  mSwapChainImages.resize(imageCount);

  vkGetSwapchainImagesKHR(vkDevice,
                          mSwapChain,
                          &imageCount,
                          mSwapChainImages.data());

  mSwapChainImageFormat = surfaceFormat.format;
  mSwapChainExtent      = extent;

  return_t result{ mSwapChainImageViews.initialize
                     (mDevice,
                      mSwapChainImages,
                      mSwapChainImageFormat,
                      VK_IMAGE_ASPECT_COLOR_BIT) };

  if(result is_eq failure)
  {
    std::string message{ ": Failed to create image views for the swap "
                         "chain." };

    PREPEND_FUNCTION_SIG_TO_STRING(message);

    executeDebugCallback(error, message);

    return failure;
  }

  return success;
}

so::vk::SwapChain::~SwapChain() noexcept
{
  destroyMembers();
}

so::vk::SwapChain&
so::vk::SwapChain::operator=(so::vk::SwapChain&& other) noexcept
{
  if(this is_eq &other)
  {
    return *this;
  }

  destroyMembers();

  mSwapChain            = other.mSwapChain;
  mSwapChainExtent      = other.mSwapChainExtent;
  mSwapChainImageFormat = other.mSwapChainImageFormat;
  mSwapChainImages      = other.mSwapChainImages;
  mSwapChainImageViews  = std::move(other.mSwapChainImageViews);
  mDevice               = other.mDevice;

  other.mSwapChain            = VK_NULL_HANDLE;
  other.mSwapChainExtent      = { 0, 0 };
  other.mSwapChainImageFormat = VK_FORMAT_UNDEFINED;
  other.mSwapChainImages      = std::vector<VkImage>();
  other.mDevice               = LogicalDevice::getSharedPtrNullDevice();
  other.mSwapChainImageViews  = ImageViews();

  return *this;
}

VkSurfaceFormatKHR
so::vk::SwapChain::chooseSwapSurfaceFormat
(std::vector<VkSurfaceFormatKHR> const& availableFormats)
{
  bool const onlyOneFormat{ availableFormats.size() is_eq 1 };
  bool const formatIsUndefined{ availableFormats[0].format is_eq
                                VK_FORMAT_UNDEFINED };

  if(onlyOneFormat and formatIsUndefined)
  {
    return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
  }

  for(auto const& availableFormat : availableFormats)
  {
    bool const hasBGR8BitFormat{ availableFormat.format is_eq
                                 VK_FORMAT_B8G8R8A8_UNORM };
    bool const supportsSRGB{ availableFormat.colorSpace is_eq
                             VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };

    if(hasBGR8BitFormat and supportsSRGB)
    {
      return availableFormat;
    }
  }

  return availableFormats[0];
}

VkPresentModeKHR
so::vk::SwapChain::chooseSwapPresentMode
  (std::vector<VkPresentModeKHR> const& availablePresentModes)
{
  VkPresentModeKHR bestMode{ VK_PRESENT_MODE_FIFO_KHR};

  for(auto const& availablePresentMode : availablePresentModes)
  {
    if(availablePresentMode is_eq VK_PRESENT_MODE_MAILBOX_KHR)
    {
      bestMode = availablePresentMode;
    }
    else if(availablePresentMode is_eq VK_PRESENT_MODE_IMMEDIATE_KHR)
    {
      bestMode = availablePresentMode;
    }
  }

  return bestMode;
}

VkExtent2D
so::vk::SwapChain::chooseSwapExtent
  (VkSurfaceCapabilitiesKHR const& capabilities, Surface const& surface)
{
  uint32_t const widthCapabilities{ capabilities.currentExtent.width };

  VkExtent2D swapExtent;

  if(widthCapabilities not_eq std::numeric_limits<uint32_t>::max())
  {
    swapExtent = capabilities.currentExtent;
  }
  else
  {
    size_type width, height;

    surface.getWindowSize(width, height);

    VkExtent2D actualExtent =
    {
      static_cast<uint32_t>(width),
      static_cast<uint32_t>(height)
    };

    actualExtent.width = std::max
    (
      capabilities.minImageExtent.width,
      std::min(capabilities.maxImageExtent.width, actualExtent.width)
    );

    actualExtent.height = std::max
    (
      capabilities.minImageExtent.height,
      std::min(capabilities.maxImageExtent.height, actualExtent.height)
    );

    swapExtent = actualExtent;
  }

  return swapExtent;
}

void
so::vk::SwapChain::destroyMembers()
{
  VkDevice device{ mDevice->getVkDevice() };

  if((mSwapChain not_eq VK_NULL_HANDLE) and (device not_eq VK_NULL_HANDLE))
  {
    vkDestroySwapchainKHR(device, mSwapChain, nullptr);
  }
}

