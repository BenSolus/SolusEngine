/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vkTextureSampler.hpp>

#include <ccException.hpp>

vk::eng::TextureSampler::TextureSampler()
  : mSampler(VK_NULL_HANDLE), mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE)
{}

vk::eng::TextureSampler::TextureSampler(SharedPtrLogicalDevice const& device)
  : mSampler(VK_NULL_HANDLE), mDevice(device)
{
  VkSamplerCreateInfo samplerInfo = {};

  samplerInfo.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  samplerInfo.magFilter               = VK_FILTER_LINEAR;
  samplerInfo.minFilter               = VK_FILTER_LINEAR;
  samplerInfo.addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.anisotropyEnable        = VK_TRUE;
  samplerInfo.maxAnisotropy           = 16;
  samplerInfo.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
  samplerInfo.unnormalizedCoordinates = VK_FALSE;
  samplerInfo.compareEnable           = VK_FALSE;
  samplerInfo.compareOp               = VK_COMPARE_OP_ALWAYS;
  samplerInfo.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;

  if(vkCreateSampler(device->getVkDevice(), &samplerInfo, nullptr, &mSampler) != VK_SUCCESS)
  {
    cc::throw_with_nested<std::runtime_error>("failed to create texture "
                                              "sampler!",
                                              PRETTY_FUNCTION_SIG);
  }
}

vk::eng::TextureSampler::~TextureSampler() noexcept  { destroyMembers(); }

void
vk::eng::TextureSampler::destroyMembers()
{
  VkDevice device(mDevice->getVkDevice());

  if((device != VK_NULL_HANDLE) && (mSampler != VK_NULL_HANDLE))
    vkDestroySampler(device, mSampler, nullptr);
}