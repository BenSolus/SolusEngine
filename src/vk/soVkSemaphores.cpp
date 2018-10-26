/* Copyright (C) 2017-2018 by Bennet Carstensen
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

#include "soVkSemaphores.hpp"

#include "soDebugCallback.hpp"

so::vk::Semaphores::Semaphores()
  : mSemaphores(),
    mDevice(LogicalDevice::getSharedPtrNullDevice())
{}

so::vk::Semaphores::~Semaphores() noexcept
{
  destroyMembers();
}

so::vk::Semaphores&
so::vk::Semaphores::operator=(Semaphores&& other) noexcept
{
  if(this is_eq &other)
  {
    return *this;
  }

  destroyMembers();

  mSemaphores = other.mSemaphores;
  mDevice     = other.mDevice;

  other.mSemaphores.resize(0);
  other.mDevice = LogicalDevice::getSharedPtrNullDevice();

  return *this;
}


so::return_t
so::vk::Semaphores::initialize(SharedPtrLogicalDevice const& device,
                               size_type              const  numSemaphores)
{
  mDevice = device;

  mSemaphores.resize(numSemaphores);

  VkSemaphoreCreateInfo semaphoreInfo{};

  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  for(auto& semaphore : mSemaphores)
  {
    VkResult const result(vkCreateSemaphore(device->getVkDevice(),
                                            &semaphoreInfo,
                                            nullptr,
                                            &semaphore));

    if(result not_eq VK_SUCCESS)
    {
      DEBUG_CALLBACK(error,
                     "Failed to create a semaphore.",
                     vkCreateSemaphore);

      return failure;
    }
  }

  return success;
}

void
so::vk::Semaphores::destroyMembers()
{
  VkDevice device(mDevice->getVkDevice());

  for(auto& semaphore : mSemaphores)
  {
    if((device not_eq VK_NULL_HANDLE) and (semaphore not_eq VK_NULL_HANDLE))
    {
      vkDestroySemaphore(device, semaphore, nullptr);
    }
  }
}
