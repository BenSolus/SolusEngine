/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vk/soVkPhysicalDevice.hpp>

#include <vk/soVkQueueFamilyIndices.hpp>
#include <vk/soVkSwapChainSupportDetails.hpp>

#include <utils/def/soDefinitions.hpp>
#include <utils/err/soException.hpp>

#include <set>

const std::vector<const char*> DEVICE_EXTENSIONS
  ({ VK_KHR_SWAPCHAIN_EXTENSION_NAME });

so::vk::PhysicalDevice::PhysicalDevice()
  : mPhysicalDevice(VK_NULL_HANDLE),
    mInstance(SHARED_PTR_NULL_INSTANCE)
{}

so::vk::PhysicalDevice::PhysicalDevice(SharedPtrInstance const& instance,
                                       VkSurfaceKHR             surface)
  : mPhysicalDevice(VK_NULL_HANDLE), mInstance(instance)
{
  VkInstance vkInstance(instance->getVkInstance());

  uint32_t   deviceCount(0);

  vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);

  if(deviceCount is_eq 0)
    throw utils::err::Exception<std::runtime_error>("failed to find GPUs with "
                                                    "Vulkan support!",
                                                    PRETTY_FUNCTION_SIG);

  std::vector<VkPhysicalDevice> devices(deviceCount);

  vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.data());

  for(const auto& device : devices)
    if(isDeviceSuitable(device, surface))
    {
      mPhysicalDevice = device;
      break;
    }

  if(mPhysicalDevice is_eq VK_NULL_HANDLE)
    throw utils::err::Exception<std::runtime_error>("failed to find a suitable "
                                                    "GPU!",
                                                    PRETTY_FUNCTION_SIG);
}

so::vk::PhysicalDevice::PhysicalDevice(VkPhysicalDevice         device,
                                       SharedPtrInstance const& instance)
  : mPhysicalDevice(device), mInstance(instance)
{}

bool
so::vk::PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
  uint32_t extensionCount;

  vkEnumerateDeviceExtensionProperties(device,
                                       nullptr,
                                       &extensionCount,
                                       nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);

  vkEnumerateDeviceExtensionProperties(device,
                                       nullptr,
                                       &extensionCount,
                                       availableExtensions.data());

  std::set<std::string> requiredExtensions(DEVICE_EXTENSIONS.begin(),
                                           DEVICE_EXTENSIONS.end());

  for(const auto& extension : availableExtensions)
    requiredExtensions.erase(extension.extensionName);

  return requiredExtensions.empty();
}

bool
so::vk::PhysicalDevice::isDeviceSuitable(VkPhysicalDevice device,
                                         VkSurfaceKHR     surface)
{
  QueueFamilyIndices indices(device, surface);

  bool extensionsSupported(checkDeviceExtensionSupport(device));

//  bool swapChainAdequate = false;
//
//  if(extensionsSupported)
//  {
//    fcm::vk::SwapChainSupportDetails swapChainSupport(device, surface);
//
//    swapChainAdequate = !swapChainSupport.getFormats().empty() &&
//                        !swapChainSupport.getPresentModes().empty();
//  }

  VkPhysicalDeviceFeatures supportedFeatures;

  vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

  return indices.isComplete() &&
         extensionsSupported  &&
         supportedFeatures.samplerAnisotropy;
}
