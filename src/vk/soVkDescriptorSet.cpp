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

#include <vk/soVkDescriptorSet.hpp>

so::vk::DescriptorSet::DescriptorSet() : mDescriptorSet(VK_NULL_HANDLE) {}

so::vk::DescriptorSet::DescriptorSet
  (SharedPtrLogicalDevice const& device,
   DescriptorPool&               descriptorPool,
   DescriptorSetLayout&          descriptorSetLayout,
   TextureSampler&               textureSampler,
   UniformBuffer&                uniformBuffer,
   std::string const&            textureKey)
  : mDescriptorSet(VK_NULL_HANDLE)
{
  VkDescriptorSetLayout* layouts(new VkDescriptorSetLayout(
    { descriptorSetLayout.getVkDescriptorSetLayout() }));

  VkDescriptorSetAllocateInfo allocInfo {};

  allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool     = descriptorPool.getVkDescriptorPool();
  allocInfo.descriptorSetCount = 1;
  allocInfo.pSetLayouts        = layouts;

  VkResult const result(vkAllocateDescriptorSets(device->getVkDevice(),
                                                 &allocInfo,
                                                 &mDescriptorSet));

  if(result not_eq VK_SUCCESS)
    throw utils::err::Exception<std::runtime_error>("failed to allocate "
                                                    "descriptor set!",
                                                    PRETTY_FUNCTION_SIG);

  VkDescriptorBufferInfo* viewProjectionBufferInfo
    (new VkDescriptorBufferInfo({}));

  viewProjectionBufferInfo->buffer =
    uniformBuffer.getViewProjectionUBOBuffer().getResource();
  viewProjectionBufferInfo->offset = 0;
  viewProjectionBufferInfo->range  = sizeof(ViewProjectionUBO);

  VkDescriptorBufferInfo* dynamicBufferInfo(new VkDescriptorBufferInfo({}));

  dynamicBufferInfo->buffer = uniformBuffer.getDynamicBuffer().getResource();
  dynamicBufferInfo->offset = 0;
  dynamicBufferInfo->range  = VK_WHOLE_SIZE;

  std::size_t const imageViewIdx
    (textureSampler.getTextureImages().find(textureKey)
       ->second.getImageViewIdx());

  VkImageView imageView
    (textureSampler.getTextureImageViews().getVkImageViews()[imageViewIdx]);

  VkDescriptorImageInfo* imageInfo(new VkDescriptorImageInfo({}));

  imageInfo->imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  imageInfo->imageView   = imageView;
  imageInfo->sampler     = textureSampler.getVkSampler();

  std::array<VkWriteDescriptorSet, 3> descriptorWrites {};

  descriptorWrites[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptorWrites[0].dstSet          = mDescriptorSet;
  descriptorWrites[0].dstBinding      = 0;
  descriptorWrites[0].dstArrayElement = 0;
  descriptorWrites[0].descriptorType  =
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  descriptorWrites[0].descriptorCount = 1;
  descriptorWrites[0].pBufferInfo     = viewProjectionBufferInfo;

  descriptorWrites[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptorWrites[1].dstSet          = mDescriptorSet;
  descriptorWrites[1].dstBinding      = 1;
  descriptorWrites[1].dstArrayElement = 0;
  descriptorWrites[1].descriptorType  =
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
  descriptorWrites[1].descriptorCount = 1;
  descriptorWrites[1].pBufferInfo     = dynamicBufferInfo;

  descriptorWrites[2].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptorWrites[2].dstSet          = mDescriptorSet;
  descriptorWrites[2].dstBinding      = 2;
  descriptorWrites[2].dstArrayElement = 0;
  descriptorWrites[2].descriptorType  =
    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  descriptorWrites[2].descriptorCount = 1;
  descriptorWrites[2].pImageInfo      = imageInfo;

  vkUpdateDescriptorSets(device->getVkDevice(),
                         static_cast<uint32_t>(descriptorWrites.size()),
                         descriptorWrites.data(),
                         0,
                         nullptr);

  delete(imageInfo);
  delete(viewProjectionBufferInfo);
  delete(dynamicBufferInfo);
  delete(layouts);
}
