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

#include <soVkSwapChain.hpp>

#include <soVkQueueFamilyIndices.hpp>
#include <soVkSwapChainSupportDetails.hpp>

#include <soException.hpp>

so::vk::SwapChain::SwapChain()
  : mSwapChain(VK_NULL_HANDLE),
    mSwapChainExtent({ 0, 0 }),
    mSwapChainImageFormat(VK_FORMAT_UNDEFINED),
    mSwapChainImages(),
    mDevice(LogicalDevice::SHARED_PTR_NULL_LOGICAL_DEVICE)
{}

so::vk::SwapChain::SwapChain(SharedPtrLogicalDevice const& device,
                             VkSurfaceKHR                  surface,
                             GLFWwindow*                   window,
                             VkSwapchainKHR                oldSwapChain)
  : mSwapChain(VK_NULL_HANDLE),
    mSwapChainExtent({ 0, 0 }),
    mSwapChainImageFormat(VK_FORMAT_UNDEFINED),
    mSwapChainImages(),
    mDevice(device)
{
  VkDevice         vkDevice(device->getVkDevice());
  VkPhysicalDevice physicalDevice(device->getVkPhysicalDevice());

  SwapChainSupportDetails swapChainSupport(physicalDevice, surface);

  VkSurfaceFormatKHR surfaceFormat
    (chooseSwapSurfaceFormat(swapChainSupport.getFormats()));
  VkPresentModeKHR presentMode
    (chooseSwapPresentMode(swapChainSupport.getPresentModes()));
  VkExtent2D extent(chooseSwapExtent(swapChainSupport.getCapabilities(),
                                     window));

  uint32_t imageCount =
    swapChainSupport.getCapabilities().minImageCount + 1;

  if((swapChainSupport.getCapabilities().maxImageCount > 0) and
     (imageCount > swapChainSupport.getCapabilities().maxImageCount))
  {
    imageCount = swapChainSupport.getCapabilities().maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo = {};

  createInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface          = surface;
  createInfo.minImageCount    = imageCount;
  createInfo.imageFormat      = surfaceFormat.format;
  createInfo.imageColorSpace  = surfaceFormat.colorSpace;
  createInfo.imageExtent      = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  QueueFamilyIndices indices(physicalDevice, surface);

  uint32_t queueFamilyIndices[] =
    { static_cast<uint32_t>(indices.getGraphicsFamily()),
      static_cast<uint32_t>(indices.getPresentFamily()) };

  if(indices.getGraphicsFamily() not_eq indices.getPresentFamily())
  {
    createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices   = queueFamilyIndices;
  }
  else
    createInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;


  createInfo.preTransform   =
    swapChainSupport.getCapabilities().currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode    = presentMode;
  createInfo.clipped        = VK_TRUE;
  createInfo.oldSwapchain   = oldSwapChain;

  VkResult const result(createSwapchainKHR(vkDevice,
                                           &createInfo,
                                           nullptr,
                                           &mSwapChain));

  if(result not_eq VK_SUCCESS)
    THROW_EXCEPTION("failed to create swap chain (" +
                    std::to_string(result) + ")");

  getSwapchainImagesKHR(vkDevice, mSwapChain, &imageCount, nullptr);
  
  mSwapChainImages.resize(imageCount);

  getSwapchainImagesKHR(vkDevice,
                        mSwapChain,
                        &imageCount,
                        mSwapChainImages.data());

  mSwapChainImageFormat = surfaceFormat.format;
  mSwapChainExtent      = extent;
}

so::vk::SwapChain::~SwapChain() noexcept
{
  destroyMembers();
}

VkSurfaceFormatKHR
so::vk::SwapChain::chooseSwapSurfaceFormat
  (std::vector<VkSurfaceFormatKHR> const& availableFormats)
{
  if((availableFormats.size() is_eq 1) and
     (availableFormats[0].format is_eq VK_FORMAT_UNDEFINED))
  {
    return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
  }

  for(auto const& availableFormat : availableFormats)
  {
    if((availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM) &&
       (availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR))
    {
      return availableFormat;
    }
  }

  return availableFormats[0];
}

VkPresentModeKHR
so::vk::SwapChain::chooseSwapPresentMode
  (std::vector<VkPresentModeKHR> const availablePresentModes)
{
  VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

  for(auto const& availablePresentMode : availablePresentModes)
    if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
      return availablePresentMode;
    else if(availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
      bestMode = availablePresentMode;

  return bestMode;
}

VkExtent2D
so::vk::SwapChain::chooseSwapExtent
  (const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window)
{
  if(capabilities.currentExtent.width not_eq
     std::numeric_limits<uint32_t>::max())
    return capabilities.currentExtent;
  else
  {
    int width, height;

    glfwGetWindowSize(window, &width, &height);

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

    return actualExtent;
  }
}

void
so::vk::SwapChain::destroyMembers()
{
  VkDevice device(mDevice->getVkDevice());

  if((mSwapChain != VK_NULL_HANDLE) && (device != VK_NULL_HANDLE))
    destroySwapchainKHR(device, mSwapChain, nullptr);
}

