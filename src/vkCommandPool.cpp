/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vkCommandPool.hpp>

#include <vkQueueFamilyIndices.hpp>

#include <ccException.hpp>

vk::eng::CommandPool::CommandPool()
  : mCommandPool(VK_NULL_HANDLE), mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE)
{}

vk::eng::CommandPool::CommandPool(SharedPtrLogicalDevice const& device,
                                  Surface&                      surface)
  : mCommandPool(VK_NULL_HANDLE), mDevice(device)
{
  vk::eng::QueueFamilyIndices queueFamilyIndices(device->getVkPhysicalDevice(),
                                                 surface.getVkSurfaceKHR());

  VkCommandPoolCreateInfo poolInfo({});

  poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.queueFamilyIndex =
    static_cast<uint32_t>(queueFamilyIndices.getGraphicsFamily());

  VkResult const result(vkCreateCommandPool(device->getVkDevice(),
                                            &poolInfo,
                                            nullptr,
                                            &mCommandPool));

  if(result not_eq VK_SUCCESS)
    cc::throw_with_nested<std::runtime_error>("failed to create command pool!",
                                              PRETTY_FUNCTION_SIG);
}

vk::eng::CommandPool::CommandPool(SharedPtrLogicalDevice const& device,
                                  VkCommandPool                 commandPool)
  : mCommandPool(commandPool), mDevice(device)
{}

vk::eng::CommandPool::~CommandPool() noexcept
{
  destroyMembers();
}

void
vk::eng::CommandPool::destroyMembers()
{
  VkDevice device(mDevice->getVkDevice());

  if((device not_eq VK_NULL_HANDLE) and (mCommandPool not_eq VK_NULL_HANDLE))
    vkDestroyCommandPool(device, mCommandPool, nullptr);
}

vk::eng::SharedPtrCommandPool const SHARED_PTR_NULL_COMMAND_POOL
  (std::make_shared<vk::eng::CommandPool>
     (std::make_shared<vk::eng::LogicalDevice>(),
      static_cast<VkCommandPool>(VK_NULL_HANDLE)));
