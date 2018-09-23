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

#include <soVkQueueFamilyIndices.hpp>

#include <vector>

so::vk::QueueFamilyIndices::QueueFamilyIndices(VkPhysicalDevice        device,
                                               Surface          const& surface)
  : QueueFamilyIndices()
{
  QueueFamilyIndices indices;

  uint32_t queueFamilyCount(0);

  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);

  vkGetPhysicalDeviceQueueFamilyProperties(device,
                                           &queueFamilyCount,
                                           queueFamilies.data());


  int i(0);

  for(auto const& queueFamily : queueFamilies)
  {
    uint32_t const queueCount(queueFamily.queueCount);
    
    if(queueCount > 0)
      if(queueFamily.queueFlags bitand VK_QUEUE_GRAPHICS_BIT)
        mGraphicsFamily = i;

    VkBool32 presentSupport = false;

    vkGetPhysicalDeviceSurfaceSupportKHR(device,
                                         static_cast<uint32_t>(i),
                                         surface.getVkSurfaceKHR(),
                                         &presentSupport);

    if((queueCount > 0) and presentSupport)
    {
      mPresentFamily = i;
    }

    if(indices.isComplete())
    {
      break;
    }

    ++i;
  }
}

