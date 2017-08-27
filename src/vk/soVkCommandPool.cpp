/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vk/soVkCommandPool.hpp>

#include <vk/soVkQueueFamilyIndices.hpp>

#include <utils/err/soException.hpp>

so::vk::CommandPool::CommandPool()
  : mCommandPool(VK_NULL_HANDLE), mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE)
{}

so::vk::CommandPool::CommandPool(SharedPtrLogicalDevice const& device,
                                 Surface&                      surface)
  : mCommandPool(VK_NULL_HANDLE), mDevice(device)
{
  QueueFamilyIndices queueFamilyIndices(device->getVkPhysicalDevice(),
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
    throw utils::err::Exception<std::runtime_error>("failed to create command "
                                                    "pool!",
                                                    PRETTY_FUNCTION_SIG);
}

so::vk::CommandPool::CommandPool(SharedPtrLogicalDevice const& device,
                                  VkCommandPool                 commandPool)
  : mCommandPool(commandPool), mDevice(device)
{}

so::vk::CommandPool::~CommandPool() noexcept
{
  destroyMembers();
}

void
so::vk::CommandPool::destroyMembers()
{
  VkDevice device(mDevice->getVkDevice());

  if((device not_eq VK_NULL_HANDLE) and (mCommandPool not_eq VK_NULL_HANDLE))
    vkDestroyCommandPool(device, mCommandPool, nullptr);
}

so::vk::SharedPtrCommandPool const SHARED_PTR_NULL_COMMAND_POOL
  (std::make_shared<so::vk::CommandPool>
     (std::make_shared<so::vk::LogicalDevice>(),
      static_cast<VkCommandPool>(VK_NULL_HANDLE)));
