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

#include "soVkCommandBuffers.hpp"

#include "cxx/soDefinitions.hpp"

so::vk::CommandBuffers::CommandBuffers()
  : mCommandBuffers(),
    mDevice(LogicalDevice::getSharedPtrNullDevice()),
    mCommandPool(CommandPool::getSharedPtrNullCommandPool())
{}

so::vk::CommandBuffers::~CommandBuffers() noexcept
{
  destroyMembers();
}

so::vk::CommandBuffers&
so::vk::CommandBuffers::operator=(CommandBuffers&& other) noexcept
{
  if(this is_eq &other)
  {
    return *this;
  }
          
  destroyMembers();

  mCommandBuffers = other.mCommandBuffers;
  mDevice         = other.mDevice;
  mCommandPool    = other.mCommandPool;
 
  other.mCommandBuffers = std::vector<VkCommandBuffer>();
  other.mDevice         = LogicalDevice::getSharedPtrNullDevice();
  other.mCommandPool    = CommandPool::getSharedPtrNullCommandPool();

  return *this;
}

so::return_t
so::vk::CommandBuffers::initialize(SharedPtrLogicalDevice const& device,
                                   SharedPtrCommandPool   const& commandPool)
{
  return success;
}

void
so::vk::CommandBuffers::destroyMembers()
{
  VkCommandPool commandPool(mCommandPool->getVkCommandPool());
  VkDevice      device(mDevice->getVkDevice());

  if(device not_eq VK_NULL_HANDLE)
  {
    if(commandPool not_eq VK_NULL_HANDLE)
    {
      if(mCommandBuffers.size() not_eq 0)
      {
        vkFreeCommandBuffers(device,
                             commandPool,
                             static_cast<uint32_t>(mCommandBuffers.size()),
                             mCommandBuffers.data());
      }
    }
  }
}

