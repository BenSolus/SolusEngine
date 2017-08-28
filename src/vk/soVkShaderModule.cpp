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
