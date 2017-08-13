/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vkShaderModule.hpp>

#include <ccException.hpp>
#include <ccFile.hpp>

vk::eng::ShaderModule::ShaderModule()
  : mShaderModule(VK_NULL_HANDLE), mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE)
{}

vk::eng::ShaderModule::ShaderModule(SharedPtrLogicalDevice const& device,
                                    const std::string&            file)
  : mShaderModule(VK_NULL_HANDLE), mDevice(device)
{
  std::vector<char> shaderCode;

  try
  {
    shaderCode = cc::readFile(file);
  }
  catch(...)
  {
    cc::throw_with_nested<std::runtime_error>("Caught exception:",
                                              PRETTY_FUNCTION_SIG);
  }

  VkShaderModuleCreateInfo createInfo = {};

  createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = shaderCode.size();
  createInfo.pCode    = reinterpret_cast<const uint32_t*>(shaderCode.data());

  if(vkCreateShaderModule(device->getVkDevice(), &createInfo, nullptr, &mShaderModule) != VK_SUCCESS)
  {
    cc::throw_with_nested<std::runtime_error>("failed to create shader "
                                              "module!",
                                              PRETTY_FUNCTION_SIG);
  }


}

vk::eng::ShaderModule::~ShaderModule() noexcept
{

  VkDevice device(mDevice->getVkDevice());

  if((mShaderModule != VK_NULL_HANDLE) && (device != VK_NULL_HANDLE))
    vkDestroyShaderModule(device, mShaderModule, nullptr);
}
