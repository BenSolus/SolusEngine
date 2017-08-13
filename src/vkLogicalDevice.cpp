/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vkLogicalDevice.hpp>

#include <vkQueueFamilyIndices.hpp>

#include <ccException.hpp>

#include <set>

vk::eng::LogicalDevice::LogicalDevice()
  : PhysicalDevice(),
    mDevice(VK_NULL_HANDLE),
    mGraphicsQueue(VK_NULL_HANDLE),
    mPresentQueue(VK_NULL_HANDLE)
{}

vk::eng::LogicalDevice::LogicalDevice(SharedPtrInstance const& instance,
                                      VkSurfaceKHR             surface)
  : PhysicalDevice(instance, surface),
    mDevice(VK_NULL_HANDLE),
    mGraphicsQueue(VK_NULL_HANDLE),
    mPresentQueue(VK_NULL_HANDLE)
{
  vk::eng::QueueFamilyIndices indices(mPhysicalDevice, surface);

  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

  std::set<int> uniqueQueueFamilies = { indices.getGraphicsFamily(),
                                        indices.getPresentFamily() };

  float queuePriority(1.0f);

  for(int queueFamily : uniqueQueueFamilies)
  {
    if(queueFamily < 0)
      cc::throw_with_nested<std::runtime_error>("found invalid queue family!",
                                                PRETTY_FUNCTION_SIG);

    VkDeviceQueueCreateInfo queueCreateInfo = {};

    queueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = static_cast<uint32_t>(queueFamily);
    queueCreateInfo.queueCount       = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(queueCreateInfo);
  }

  VkPhysicalDeviceFeatures deviceFeatures = {};

  deviceFeatures.samplerAnisotropy = VK_TRUE;

  VkDeviceCreateInfo createInfo = {};

  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.pQueueCreateInfos = queueCreateInfos.data();
  createInfo.queueCreateInfoCount =
    static_cast<uint32_t>(queueCreateInfos.size());
  createInfo.pEnabledFeatures = &deviceFeatures;
  createInfo.enabledExtensionCount = 0;
  createInfo.enabledExtensionCount =
    static_cast<uint32_t>(DEVICE_EXTENSIONS.size());
  createInfo.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();

  if(ENABLE_VALIDATION_LAYERS)
  {
    createInfo.enabledLayerCount =
      static_cast<uint32_t>(VALIDATION_LAYERS.size());
    createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
  } else
      createInfo.enabledLayerCount = 0;

  if(vkCreateDevice(mPhysicalDevice, &createInfo, nullptr, &mDevice) != VK_SUCCESS)
  {
    cc::throw_with_nested<std::runtime_error>("failed to create logical "
                                              "device!",
                                              PRETTY_FUNCTION_SIG);
  }

  vkGetDeviceQueue(mDevice,
                   static_cast<uint32_t>(indices.getGraphicsFamily()),
                   0,
                   &mGraphicsQueue);
  vkGetDeviceQueue(mDevice,
                   static_cast<uint32_t>(indices.getPresentFamily()),
                   0,
                   &mPresentQueue);
}

vk::eng::LogicalDevice::~LogicalDevice() noexcept { destroyMembers(); }

void
vk::eng::LogicalDevice::destroyMembers()
{
  if(mDevice != VK_NULL_HANDLE)
    vkDestroyDevice(mDevice, nullptr);
}

vk::eng::SharedPtrLogicalDevice const SHARED_PTR_NULL_LOGICAL_DEVICE
  (std::make_shared<vk::eng::LogicalDevice>());
