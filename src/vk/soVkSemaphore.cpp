/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vk/soVkSemaphore.hpp>

#include <utils/err/soException.hpp>

so::vk::Semaphore::Semaphore()
  : mSemaphore(VK_NULL_HANDLE), mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE)
{}

so::vk::Semaphore::Semaphore(SharedPtrLogicalDevice const& device)
  : mSemaphore(VK_NULL_HANDLE), mDevice(device)
{
  VkSemaphoreCreateInfo semaphoreInfo = {};

  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkResult const result(vkCreateSemaphore(device->getVkDevice(),
                                          &semaphoreInfo,
                                          nullptr,
                                          &mSemaphore));

  if(result not_eq VK_SUCCESS)
    throw utils::err::Exception<std::runtime_error>("failed to create "
                                                    "semaphores!",
                                                    PRETTY_FUNCTION_SIG);
}

so::vk::Semaphore::~Semaphore() noexcept
{
  destroyMembers();
}

void
so::vk::Semaphore::destroyMembers()
{
  VkDevice device(mDevice->getVkDevice());

  if((device not_eq VK_NULL_HANDLE) and (mSemaphore not_eq VK_NULL_HANDLE))
    vkDestroySemaphore(device, mSemaphore, nullptr);
}