/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vkDescriptorPool.hpp>

#include <ccException.hpp>

vk::eng::DescriptorPool::DescriptorPool()
  : mDescriptorPool(VK_NULL_HANDLE), mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE)
{}

vk::eng::DescriptorPool::DescriptorPool(SharedPtrLogicalDevice const& device)
  : mDescriptorPool(VK_NULL_HANDLE), mDevice(device)
{
  std::array<VkDescriptorPoolSize, 2> poolSizes = {};

  poolSizes[0].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  poolSizes[0].descriptorCount = 1;
  poolSizes[1].type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  poolSizes[1].descriptorCount = 1;

  VkDescriptorPoolCreateInfo poolInfo = {};

  poolInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
  poolInfo.pPoolSizes    = poolSizes.data();
  poolInfo.maxSets       = 1;

  if(vkCreateDescriptorPool(device->getVkDevice(), &poolInfo, nullptr, &mDescriptorPool) != VK_SUCCESS)
  {
    cc::throw_with_nested<std::runtime_error>("failed to create descriptor "
                                              "pool!",
                                              PRETTY_FUNCTION_SIG);
  }
}

vk::eng::DescriptorPool::~DescriptorPool() noexcept
{
  destroyMembers();
}

void
vk::eng::DescriptorPool::destroyMembers()
{
  VkDevice device(mDevice->getVkDevice());

  if((device != VK_NULL_HANDLE) && (mDescriptorPool != VK_NULL_HANDLE))
    vkDestroyDescriptorPool(device, mDescriptorPool, nullptr);
}
