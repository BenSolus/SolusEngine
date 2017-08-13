/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vkDescriptorSet.hpp>

#include <vkUniformBuffer.hpp>

#include <ccException.hpp>

vk::eng::DescriptorSet::DescriptorSet() : mDescriptorSet(VK_NULL_HANDLE) {}

vk::eng::DescriptorSet::DescriptorSet(VkDescriptorSetLayout descriptorSetLayout,
                                      VkDescriptorPool      descriptorPool,
                                      VkDevice              device,
                                      VkBuffer              uniformBuffer,
                                      VkImageView           textureImageView,
                                      VkSampler             textureSampler)
  : mDescriptorSet(VK_NULL_HANDLE)
{
  VkDescriptorSetLayout layouts[] = { descriptorSetLayout };

  VkDescriptorSetAllocateInfo allocInfo = {};

  allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool     = descriptorPool;
  allocInfo.descriptorSetCount = 1;
  allocInfo.pSetLayouts        = layouts;

  if(vkAllocateDescriptorSets(device, &allocInfo, &mDescriptorSet) != VK_SUCCESS)
  {
    cc::throw_with_nested<std::runtime_error>("failed to allocate descriptor "
                                              "set!",
                                              PRETTY_FUNCTION_SIG);
  }

  VkDescriptorBufferInfo bufferInfo = {};

  bufferInfo.buffer = uniformBuffer;
  bufferInfo.offset = 0;
  bufferInfo.range  = sizeof(UniformBufferObject);

  VkDescriptorImageInfo imageInfo = {};

  imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  imageInfo.imageView   = textureImageView;
  imageInfo.sampler     = textureSampler;

  std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

  descriptorWrites[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptorWrites[0].dstSet          = mDescriptorSet;
  descriptorWrites[0].dstBinding      = 0;
  descriptorWrites[0].dstArrayElement = 0;
  descriptorWrites[0].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  descriptorWrites[0].descriptorCount = 1;
  descriptorWrites[0].pBufferInfo     = &bufferInfo;

  descriptorWrites[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptorWrites[1].dstSet          = mDescriptorSet;
  descriptorWrites[1].dstBinding      = 1;
  descriptorWrites[1].dstArrayElement = 0;
  descriptorWrites[1].descriptorType  =
    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  descriptorWrites[1].descriptorCount = 1;
  descriptorWrites[1].pImageInfo      = &imageInfo;

  vkUpdateDescriptorSets(device,
                         static_cast<uint32_t>(descriptorWrites.size()),
                         descriptorWrites.data(),
                         0,
                         nullptr);
}
