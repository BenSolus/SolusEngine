/*
 * Copyright (C) 2017-2018 by Bennet Carstensen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <soVkPipeline.hpp>

#include <soVkShaderModule.hpp>

#include <soException.hpp>
#include <soFileSystem.hpp>

so::vk::Pipeline::Pipeline()
  : mPipeline(VK_NULL_HANDLE),
    mPipelineLayout(VK_NULL_HANDLE),
    mDevice(LogicalDevice::SHARED_PTR_NULL_LOGICAL_DEVICE)
{}

so::vk::Pipeline::Pipeline(SharedPtrLogicalDevice const& device,
                            VkExtent2D                    swapChainExtent,
                            VkRenderPass                  renderPass,
                            VkDescriptorSetLayout&        descriptorSetLayout)
: mPipeline(VK_NULL_HANDLE),
  mPipelineLayout(VK_NULL_HANDLE),
  mDevice(device)
{
  ShaderModule vertShader, fragShader;

  try
  {
    vertShader = ShaderModule(device,
                              BIN_DIR + "/data/shaders/triangle/vert.spv");

    fragShader = ShaderModule(device,
                              BIN_DIR + "/data/shaders/triangle/frag.spv");
  }
  catch(...)
  {
    THROW_NESTED_EXCEPTION("Caught exception:");
  }

  VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};

  vertShaderStageInfo.sType  =
    VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertShaderStageInfo.stage  = VK_SHADER_STAGE_VERTEX_BIT;
  vertShaderStageInfo.module = vertShader.getVkShaderModule();
  vertShaderStageInfo.pName  = "triangle";

  VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};

  fragShaderStageInfo.sType  =
    VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  fragShaderStageInfo.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragShaderStageInfo.module = fragShader.getVkShaderModule();
  fragShaderStageInfo.pName  = "triangle";

  VkPipelineShaderStageCreateInfo shaderStages[] =
    { vertShaderStageInfo, fragShaderStageInfo };

  (void) shaderStages;

  (void) swapChainExtent;
  (void) renderPass;
  (void) descriptorSetLayout;
} 

so::vk::Pipeline::~Pipeline() noexcept
{
  destroyMembers();
}

void
so::vk::Pipeline::destroyMembers()
{
  VkDevice device(mDevice->getVkDevice());

  if(device not_eq VK_NULL_HANDLE)
  {
    if(mPipeline not_eq VK_NULL_HANDLE)
      destroyPipeline(device, mPipeline, nullptr);

    if(mPipelineLayout not_eq VK_NULL_HANDLE)
     destroyPipelineLayout(device, mPipelineLayout, nullptr);
  }
}
