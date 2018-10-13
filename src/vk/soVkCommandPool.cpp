/*
 * Copyright (C) 2017-2018 by Bennet Carstensen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "soVkCommandPool.hpp"

#include "soVkQueueFamilyIndices.hpp"

#include "cxx/soDebugCallback.hpp"
#include "cxx/soDefinitions.hpp"

so::vk::SharedPtrCommandPool const&
so::vk::CommandPool::getSharedPtrNullCommandPool()
{
  static SharedPtrCommandPool commandPool{ std::make_shared<CommandPool>() };

  return commandPool;
}

so::vk::CommandPool::CommandPool()
  : mCommandPool(VK_NULL_HANDLE),
    mDevice(LogicalDevice::getSharedPtrNullDevice())
{}

so::vk::CommandPool::~CommandPool() noexcept
{
	destroyMembers();
}

so::vk::CommandPool&
so::vk::CommandPool::operator=(CommandPool&& other) noexcept
{
	if(this is_eq &other)
	{
		return *this;
	}

  destroyMembers();

  mCommandPool = other.mCommandPool;
  mDevice      = other.mDevice;

  other.mCommandPool = VK_NULL_HANDLE;
  other.mDevice      = LogicalDevice::getSharedPtrNullDevice();

  return *this;
}

so::return_t
so::vk::CommandPool::initialize(SharedPtrLogicalDevice const& device,
                                Surface                const& surface)
{
  mDevice = device;

  auto vkPhysicalDevice{ mDevice->getVkPhysicalDevice() };

  QueueFamilyIndices queueFamilyIndices{ vkPhysicalDevice, surface };

  uint32_t graphicsFamily{ static_cast<uint32_t>
                             (queueFamilyIndices.getGraphicsFamily()) };

  VkCommandPoolCreateInfo poolInfo{};

  poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.queueFamilyIndex = graphicsFamily;
  poolInfo.flags            = 0; // Optional

  auto vkDevice{ mDevice->getVkDevice() };

  auto result{ vkCreateCommandPool(vkDevice,
                                   &poolInfo,
                                   nullptr,
                                   &mCommandPool) };

  if(result not_eq VK_SUCCESS)
  {
    std::string message{ "Failed to create a command pool." };

    DEBUG_CALLBACK(error, message, vkCreateCommandPool);

    return failure;
  }

  return success;
}

void
so::vk::CommandPool::destroyMembers()
{
  VkDevice device(mDevice->getVkDevice());

  if((device not_eq VK_NULL_HANDLE) and (mCommandPool not_eq VK_NULL_HANDLE))
  {
    vkDestroyCommandPool(device, mCommandPool, nullptr);
  }
}

