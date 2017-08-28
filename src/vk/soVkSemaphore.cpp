/*
 * Copyright (C) 2017 by Bennet Carstensen
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