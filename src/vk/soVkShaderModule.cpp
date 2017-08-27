/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vk/soVkShaderModule.hpp>

#include <utils/err/soException.hpp>
#include <utils/fs/soFileSystem.hpp>

so::vk::ShaderModule::ShaderModule()
  : mShaderModule(VK_NULL_HANDLE), mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE)
{}

so::vk::ShaderModule::ShaderModule(SharedPtrLogicalDevice const& device,
                                    const std::string&            file)
  : mShaderModule(VK_NULL_HANDLE), mDevice(device)
{
  std::vector<char> shaderCode;

  try
  {
    shaderCode = utils::fs::readBinaryFile(file);
  }
  catch(...)
  {
    std::throw_with_nested(utils::err::Exception<std::runtime_error>
      ("Caught exception:", PRETTY_FUNCTION_SIG));
  }

  VkShaderModuleCreateInfo createInfo = {};

  createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = shaderCode.size();
  createInfo.pCode    = reinterpret_cast<const uint32_t*>(shaderCode.data());

  VkResult const result(vkCreateShaderModule(device->getVkDevice(),
                                             &createInfo,
                                             nullptr,
                                             &mShaderModule));

  if(result not_eq VK_SUCCESS)
    throw utils::err::Exception<std::runtime_error>("failed to create shader "
                                                    "module!",
                                                    PRETTY_FUNCTION_SIG);
}

so::vk::ShaderModule::~ShaderModule() noexcept
{
  destroy_members();
}

void
so::vk::ShaderModule::destroy_members()
{
  VkDevice device(mDevice->getVkDevice());

  if((mShaderModule not_eq VK_NULL_HANDLE) and (device not_eq VK_NULL_HANDLE))
    vkDestroyShaderModule(device, mShaderModule, nullptr);
}
