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

#include <soVkLogicalDevice.hpp>

#include <soVkQueueFamilyIndices.hpp>

#include <soException.hpp>

#include <set>

so::vk::LogicalDevice::LogicalDevice()
  : PhysicalDevice(),
    mDevice(VK_NULL_HANDLE),
    mGraphicsQueue(VK_NULL_HANDLE),
    mPresentQueue(VK_NULL_HANDLE)
{}

so::vk::LogicalDevice::LogicalDevice(SharedPtrInstance const& instance,
                                      VkSurfaceKHR             surface)
  : PhysicalDevice(instance, surface),
    mDevice(VK_NULL_HANDLE),
    mGraphicsQueue(VK_NULL_HANDLE),
    mPresentQueue(VK_NULL_HANDLE)
{
  QueueFamilyIndices indices(mPhysicalDevice, surface);

  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

  std::set<int> uniqueQueueFamilies = { indices.getGraphicsFamily(),
                                        indices.getPresentFamily() };

  float* queuePriority(new float(1.0f));

  for(int queueFamily : uniqueQueueFamilies)
  {
    if(queueFamily < 0)
      THROW_EXCEPTION("found invalid queue family!");

    VkDeviceQueueCreateInfo queueCreateInfo({});

    queueCreateInfo.sType            =
      VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = static_cast<uint32_t>(queueFamily);
    queueCreateInfo.queueCount       = 1;
    queueCreateInfo.pQueuePriorities = queuePriority;
    queueCreateInfos.push_back(queueCreateInfo);
  }

  VkPhysicalDeviceFeatures* deviceFeatures(new VkPhysicalDeviceFeatures({}));

  deviceFeatures->samplerAnisotropy = VK_TRUE;

  VkDeviceCreateInfo createInfo({});

  createInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.pQueueCreateInfos       = queueCreateInfos.data();
  createInfo.queueCreateInfoCount    =
    static_cast<uint32_t>(queueCreateInfos.size());
  createInfo.pEnabledFeatures        = deviceFeatures;
  createInfo.enabledExtensionCount   =
    static_cast<uint32_t>(DEVICE_EXTENSIONS.size());
  createInfo.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();

  if(ENABLE_VALIDATION_LAYERS)
  {
    createInfo.enabledLayerCount =
      static_cast<uint32_t>(VALIDATION_LAYERS.size());
    createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
  } else
      createInfo.enabledLayerCount = 0;

  VkResult const result(createDevice(mPhysicalDevice,
                                     &createInfo,
                                     nullptr,
                                     &mDevice));

  delete deviceFeatures;
  delete queuePriority;

  if(result not_eq VK_SUCCESS)
    THROW_EXCEPTION("failed to create logical device (" +
                    std::to_string(result) +
                    ").");

  getDeviceQueue(mDevice,
                 static_cast<uint32_t>(indices.getGraphicsFamily()),
                 0,
                 &mGraphicsQueue);
  getDeviceQueue(mDevice,
                 static_cast<uint32_t>(indices.getPresentFamily()),
                 0,
                 &mPresentQueue);
}

so::vk::LogicalDevice::~LogicalDevice() noexcept { destroyMembers(); }

so::vk::LogicalDevice&
so::vk::LogicalDevice::operator=(so::vk::LogicalDevice&& other) noexcept
{
  if(this is_eq &other)
    return *this;

  PhysicalDevice::operator=(static_cast<PhysicalDevice&&>(other));

  destroyMembers();

  mDevice        = other.mDevice;
  mGraphicsQueue = other.mGraphicsQueue;
  mPresentQueue  = other.mPresentQueue;

  other.mDevice        = VK_NULL_HANDLE;
  other.mGraphicsQueue = VK_NULL_HANDLE;
  other.mPresentQueue  = VK_NULL_HANDLE;

  return *this;
}


void
so::vk::LogicalDevice::destroyMembers()
{
  if(mDevice not_eq VK_NULL_HANDLE)
    destroyDevice(mDevice, nullptr);
}

so::vk::SharedPtrLogicalDevice const
so::vk::LogicalDevice::SHARED_PTR_NULL_LOGICAL_DEVICE
  (std::make_shared<so::vk::LogicalDevice>());
