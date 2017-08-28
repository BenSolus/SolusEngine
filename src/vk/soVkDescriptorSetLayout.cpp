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

#include <vk/soVkDescriptorSetLayout.hpp>

#include <utils/err/soException.hpp>

so::vk::DescriptorSetLayout::DescriptorSetLayout()
  : mDescriptorSetLayout(VK_NULL_HANDLE),
    mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE)
{}

so::vk::DescriptorSetLayout::DescriptorSetLayout
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

  VkResult const result(vkCreateDescriptorSetLayout(device->getVkDevice(),
                                                    &layoutInfo,
                                                    nullptr,
                                                    &mDescriptorSetLayout));

  if(result not_eq VK_SUCCESS)
    throw utils::err::Exception<std::runtime_error>("failed to create "
                                                    "descriptor set layout!",
                                                    PRETTY_FUNCTION_SIG);
}

so::vk::DescriptorSetLayout::~DescriptorSetLayout() noexcept
{
  destroyMembers();
}

void
so::vk::DescriptorSetLayout::destroyMembers()
{
  VkDevice device(mDevice->getVkDevice());

  if((device != VK_NULL_HANDLE) && (mDescriptorSetLayout != VK_NULL_HANDLE))
    vkDestroyDescriptorSetLayout(device, mDescriptorSetLayout, nullptr);
}
