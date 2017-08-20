/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vkDescriptorSetLayout.hpp>

#include <ccException.hpp>

vk::eng::DescriptorSetLayout::DescriptorSetLayout()
  : mDescriptorSetLayout(VK_NULL_HANDLE),
    mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE)
{}

vk::eng::DescriptorSetLayout::DescriptorSetLayout
  (SharedPtrLogicalDevice const& device)
  : mDescriptorSetLayout(VK_NULL_HANDLE),
    mDevice(device)
{
  VkDescriptorSetLayoutBinding uboViewProjectionLayoutBinding = {};

  uboViewProjectionLayoutBinding.binding            = 0;
  uboViewProjectionLayoutBinding.descriptorCount    = 1;
  uboViewProjectionLayoutBinding.descriptorType     =
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  uboViewProjectionLayoutBinding.pImmutableSamplers = nullptr;
  uboViewProjectionLayoutBinding.stageFlags         =
    VK_SHADER_STAGE_VERTEX_BIT;

  VkDescriptorSetLayoutBinding uboDynamicLayoutBinding = {};

  uboDynamicLayoutBinding.binding            = 1;
  uboDynamicLayoutBinding.descriptorCount    = 1;
  uboDynamicLayoutBinding.descriptorType     =
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
  uboDynamicLayoutBinding.pImmutableSamplers = nullptr;
  uboDynamicLayoutBinding.stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;

  VkDescriptorSetLayoutBinding samplerLayoutBinding = {};

  samplerLayoutBinding.binding            = 2;
  samplerLayoutBinding.descriptorCount    = 1;
  samplerLayoutBinding.descriptorType     =
    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  samplerLayoutBinding.pImmutableSamplers = nullptr;
  samplerLayoutBinding.stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT;

  std::array<VkDescriptorSetLayoutBinding, 3> bindings
    ({ uboViewProjectionLayoutBinding,
       uboDynamicLayoutBinding,
       samplerLayoutBinding });

  VkDescriptorSetLayoutCreateInfo layoutInfo = {};

  layoutInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
  layoutInfo.pBindings    = bindings.data();

  if(vkCreateDescriptorSetLayout(device->getVkDevice(), &layoutInfo, nullptr, &mDescriptorSetLayout) != VK_SUCCESS)
  {
    cc::throw_with_nested<std::runtime_error>("failed to create descriptor set "
                                              "layout!",
                                              PRETTY_FUNCTION_SIG);
  }
}

vk::eng::DescriptorSetLayout::~DescriptorSetLayout() noexcept
{
  destroyMembers();
}

void
vk::eng::DescriptorSetLayout::destroyMembers()
{
  VkDevice device(mDevice->getVkDevice());

  if((device != VK_NULL_HANDLE) && (mDescriptorSetLayout != VK_NULL_HANDLE))
    vkDestroyDescriptorSetLayout(device, mDescriptorSetLayout, nullptr);
}
