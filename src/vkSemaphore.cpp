/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vkSemaphore.hpp>

#include <ccException.hpp>

vk::eng::Semaphore::Semaphore()
  : mSemaphore(VK_NULL_HANDLE), mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE)
{}

vk::eng::Semaphore::Semaphore(SharedPtrLogicalDevice const& device)
  : mSemaphore(VK_NULL_HANDLE), mDevice(device)
{
  VkSemaphoreCreateInfo semaphoreInfo = {};

  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  if(vkCreateSemaphore(device->getVkDevice(), &semaphoreInfo, nullptr, &mSemaphore) != VK_SUCCESS)
  {
    cc::throw_with_nested<std::runtime_error>("failed to create "
                                              "semaphores!",
                                              PRETTY_FUNCTION_SIG);
  }
}

vk::eng::Semaphore::~Semaphore() noexcept
{
  VkDevice device(mDevice->getVkDevice());

  if((device != VK_NULL_HANDLE) && (mSemaphore != VK_NULL_HANDLE))
    vkDestroySemaphore(device, mSemaphore, nullptr);
}
