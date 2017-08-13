/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vkQueueFamilyIndices.hpp>

#include <vector>

vk::eng::QueueFamilyIndices::QueueFamilyIndices(VkPhysicalDevice device,
                                                VkSurfaceKHR     surface)
  : mGraphicsFamily(-1), mPresentFamily(-1)
{
  vk::eng::QueueFamilyIndices indices;

  uint32_t queueFamilyCount = 0;

  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);

  vkGetPhysicalDeviceQueueFamilyProperties(device,
                                           &queueFamilyCount,
                                           queueFamilies.data());


  int i = 0;

  for(const auto& queueFamily : queueFamilies)
  {
    if(queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
      mGraphicsFamily = i;

    VkBool32 presentSupport = false;

    vkGetPhysicalDeviceSurfaceSupportKHR(device,
                                         static_cast<uint32_t>(i),
                                         surface,
                                         &presentSupport);

    if((queueFamily.queueCount > 0) && presentSupport)
      mPresentFamily = i;

    if(indices.isComplete())
      break;

    ++i;
  }
}
