/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vk/soVkSwapChain.hpp>

#include <vk/soVkQueueFamilyIndices.hpp>
#include <vk/soVkSwapChainSupportDetails.hpp>

#include <utils/err/soException.hpp>

so::vk::SwapChain::SwapChain()
  : mSwapChain(VK_NULL_HANDLE),
    mSwapChainExtent({ 0, 0 }),
    mSwapChainImageFormat(VK_FORMAT_UNDEFINED),
    mSwapChainImages(),
    mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE)
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

  if((swapChainSupport.getCapabilities().maxImageCount > 0) &&
     (imageCount > swapChainSupport.getCapabilities().maxImageCount))
  {
    imageCount = swapChainSupport.getCapabilities().maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo = {};

  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = surface;
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  QueueFamilyIndices indices(physicalDevice, surface);

  uint32_t queueFamilyIndices[] =
    { static_cast<uint32_t>(indices.getGraphicsFamily()),
      static_cast<uint32_t>(indices.getPresentFamily()) };

  if(indices.getGraphicsFamily() != indices.getPresentFamily())
  {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
  }
  else
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;


  createInfo.preTransform   =
    swapChainSupport.getCapabilities().currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode    = presentMode;
  createInfo.clipped        = VK_TRUE;
  createInfo.oldSwapchain   = oldSwapChain;

  VkResult const result(vkCreateSwapchainKHR(vkDevice,
                                             &createInfo,
                                             nullptr,
                                             &mSwapChain));

  if(result not_eq VK_SUCCESS)
    throw utils::err::Exception<std::runtime_error>("failed to create swap "
                                                    "chain!",
                                                    PRETTY_FUNCTION_SIG);

  vkGetSwapchainImagesKHR(vkDevice, mSwapChain, &imageCount, nullptr);
  mSwapChainImages.resize(imageCount);

  vkGetSwapchainImagesKHR(vkDevice,
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
  (const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
  if((availableFormats.size() == 1) &&
     (availableFormats[0].format == VK_FORMAT_UNDEFINED))
  {
    return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
  }

  for(const auto& availableFormat : availableFormats)
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
  (const std::vector<VkPresentModeKHR> availablePresentModes)
{
  VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

  for(const auto& availablePresentMode : availablePresentModes)
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
  if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
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
    vkDestroySwapchainKHR(device, mSwapChain, nullptr);
}
