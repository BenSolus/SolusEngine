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

#include <soVkPhysicalDevice.hpp>

#include <soVkQueueFamilyIndices.hpp>
#include <soVkSwapChainSupportDetails.hpp>

#include <soDefinitions.hpp>
#include <soException.hpp>

#include <set>

std::vector<char const*> const DEVICE_EXTENSIONS
  ({ VK_KHR_SWAPCHAIN_EXTENSION_NAME });

so::vk::PhysicalDevice::PhysicalDevice()
  : mPhysicalDevice(VK_NULL_HANDLE),
    mInstance(Instance::SHARED_PTR_NULL_INSTANCE)
{}

so::vk::PhysicalDevice::PhysicalDevice(SharedPtrInstance const& instance,
                                       Surface           const& surface)
  : mPhysicalDevice(VK_NULL_HANDLE), mInstance(instance)
{
  VkInstance vkInstance(instance->getVkInstance());

  uint32_t   deviceCount(0);

  enumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);

  if(deviceCount is_eq 0)
    THROW_EXCEPTION("failed to find GPUs with Vulkan ""support!");

  std::vector<VkPhysicalDevice> devices(deviceCount);

  enumeratePhysicalDevices(vkInstance, &deviceCount, devices.data());

  for(auto const& device : devices)
  {
    if(isDeviceSuitable(device, surface))
    {
      mPhysicalDevice = device;
      break;
    }
  }

  if(mPhysicalDevice is_eq VK_NULL_HANDLE)
    THROW_EXCEPTION("failed to find a suitable GPU!");
}

so::vk::PhysicalDevice::PhysicalDevice(VkPhysicalDevice         device,
                                       SharedPtrInstance const& instance)
  : mPhysicalDevice(device), mInstance(instance)
{}

so::vk::PhysicalDevice&
so::vk::PhysicalDevice::operator=(PhysicalDevice&& other) noexcept
{
  if(this is_eq &other)
    return *this;

  mPhysicalDevice = other.mPhysicalDevice;
  mInstance       = other.mInstance;

  other.mPhysicalDevice = VK_NULL_HANDLE;
  other.mInstance       = Instance::SHARED_PTR_NULL_INSTANCE;

  return *this;
}

bool
so::vk::PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
  uint32_t extensionCount;

  enumerateDeviceExtensionProperties(device,
                                     nullptr,
                                     &extensionCount,
                                     nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);

  enumerateDeviceExtensionProperties(device,
                                     nullptr,
                                     &extensionCount,
                                     availableExtensions.data());

  std::set<std::string> requiredExtensions(DEVICE_EXTENSIONS.begin(),
                                           DEVICE_EXTENSIONS.end());

  for(auto const& extension : availableExtensions)
    requiredExtensions.erase(extension.extensionName);

  return requiredExtensions.empty();
}

bool
so::vk::PhysicalDevice::isDeviceSuitable(VkPhysicalDevice        device,
                                         Surface          const& surface)
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

  getPhysicalDeviceFeatures(device, &supportedFeatures);

  return indices.isComplete() &&
         extensionsSupported  &&
         supportedFeatures.samplerAnisotropy;
}
