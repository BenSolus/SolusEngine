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

#include "soVkPipeline.hpp"

#include "soVkShaderModule.hpp"

#include "cxx/soDebugCallback.hpp"
#include "cxx/soFileSystem.hpp"

so::vk::Pipeline::Pipeline()
  : mPipeline(VK_NULL_HANDLE),
    mPipelineLayout(VK_NULL_HANDLE),
    mDevice(LogicalDevice::getSharedPtrNullDevice())
{}

so::vk::Pipeline::~Pipeline() noexcept
{
  destroyMembers();
}

so::vk::Pipeline&
so::vk::Pipeline::operator=(Pipeline&& other) noexcept
{
  if(this is_eq &other)
  {
    return *this;
  }

  destroyMembers();

  mPipeline       = other.mPipeline;
  mPipelineLayout = other.mPipelineLayout;
  mDevice         = other.mDevice;

  other.mPipeline       = VK_NULL_HANDLE;
  other.mPipelineLayout = VK_NULL_HANDLE;
  other.mDevice         = LogicalDevice::getSharedPtrNullDevice();

  return *this;
}

so::return_t
so::vk::Pipeline::initialize(SharedPtrLogicalDevice const& device,
                             SwapChain              const& swapChain,
														 RenderPass             const& renderPass)
{
  mDevice = device;

  ShaderModule vertShader{ device,
                           BIN_DIR + "/data/shaders/triangle/vert.spv" };

  ShaderModule fragShader{ device,
                           BIN_DIR + "/data/shaders/triangle/frag.spv" };

  bool const gotValidShaders
               { (vertShader.getVkShaderModule() not_eq VK_NULL_HANDLE) and
                 (fragShader.getVkShaderModule() not_eq VK_NULL_HANDLE) };
 
  if(not gotValidShaders)
  {
    DEBUG_CALLBACK(error, "Failed to load shader code.");

    return failure;
  }

  VkPipelineShaderStageCreateInfo vertShaderStageInfo{};

  vertShaderStageInfo.sType  =
    VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertShaderStageInfo.stage  = VK_SHADER_STAGE_VERTEX_BIT;
  vertShaderStageInfo.module = vertShader.getVkShaderModule();
  vertShaderStageInfo.pName  = "main";

  VkPipelineShaderStageCreateInfo fragShaderStageInfo{};

  fragShaderStageInfo.sType  =
    VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  fragShaderStageInfo.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragShaderStageInfo.module = fragShader.getVkShaderModule();
  fragShaderStageInfo.pName  = "main";

  VkPipelineShaderStageCreateInfo shaderStages[]{ vertShaderStageInfo,
                                                  fragShaderStageInfo };

  VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
  
  vertexInputInfo.sType                           =
    VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputInfo.vertexBindingDescriptionCount   = 0;
  vertexInputInfo.pVertexBindingDescriptions      = nullptr;
  vertexInputInfo.vertexAttributeDescriptionCount = 0;
  vertexInputInfo.pVertexAttributeDescriptions 		= nullptr;

  VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
  
  inputAssembly.sType                  =
    VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssembly.primitiveRestartEnable = VK_FALSE;

  VkViewport viewport{};

  viewport.x        = 0.0f;
  viewport.y        = 0.0f;
  viewport.width    = static_cast<float>(swapChain.getVkExtent().width);
  viewport.height   = static_cast<float>(swapChain.getVkExtent().height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f; 

  VkRect2D scissor{};

  scissor.offset = { 0, 0 };
  scissor.extent = swapChain.getVkExtent();

  VkPipelineViewportStateCreateInfo viewportState{};

  viewportState.sType         =
    VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.viewportCount = 1;
  viewportState.pViewports    = &viewport;
  viewportState.scissorCount  = 1;
  viewportState.pScissors     = &scissor;

  VkPipelineRasterizationStateCreateInfo rasterizer{};

  rasterizer.sType                   =
    VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable        = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode             = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth               = 1.0f;
  rasterizer.cullMode                = VK_CULL_MODE_BACK_BIT;
  rasterizer.frontFace               = VK_FRONT_FACE_CLOCKWISE;
  rasterizer.depthBiasEnable         = VK_FALSE;
  rasterizer.depthBiasConstantFactor = 0.0f; // Optional
  rasterizer.depthBiasClamp          = 0.0f; // Optional
  rasterizer.depthBiasSlopeFactor    = 0.0f; // Optionali

  VkPipelineMultisampleStateCreateInfo multisampling{};

  multisampling.sType                 =
    VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable   = VK_FALSE;
  multisampling.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
  multisampling.minSampleShading      = 1.0f;     // Optional
  multisampling.pSampleMask           = nullptr;  // Optional
  multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
  multisampling.alphaToOneEnable      = VK_FALSE; // Optional

  VkPipelineColorBlendAttachmentState colorBlendAttachment{};

  colorBlendAttachment.colorWriteMask      = VK_COLOR_COMPONENT_R_BIT bitor
                                             VK_COLOR_COMPONENT_G_BIT bitor
                                             VK_COLOR_COMPONENT_B_BIT bitor
                                             VK_COLOR_COMPONENT_A_BIT;
  colorBlendAttachment.blendEnable         = VK_FALSE;
  colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
  colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
  colorBlendAttachment.colorBlendOp        = VK_BLEND_OP_ADD;      // Optional
  colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
  colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
  colorBlendAttachment.alphaBlendOp        = VK_BLEND_OP_ADD;      // Optional

  VkPipelineColorBlendStateCreateInfo colorBlending{};

  colorBlending.sType             =
    VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlending.logicOpEnable     = VK_FALSE;
  colorBlending.logicOp           = VK_LOGIC_OP_COPY; // Optional
  colorBlending.attachmentCount   = 1;
  colorBlending.pAttachments      = &colorBlendAttachment;
  colorBlending.blendConstants[0] = 0.0f; // Optional
  colorBlending.blendConstants[1] = 0.0f; // Optional
  colorBlending.blendConstants[2] = 0.0f; // Optional

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};

  pipelineLayoutInfo.sType                  =
    VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount         = 0;       // Optional
  pipelineLayoutInfo.pSetLayouts            = nullptr; // Optional
  pipelineLayoutInfo.pushConstantRangeCount = 0;       // Optional
  pipelineLayoutInfo.pPushConstantRanges    = nullptr; // Optionali

  auto vkDevice{ mDevice->getVkDevice() };

  auto result{ vkCreatePipelineLayout(vkDevice,
                                      &pipelineLayoutInfo,
                                      nullptr,
                                      &mPipelineLayout) };
  
  if(result not_eq VK_SUCCESS)
  {
    DEBUG_CALLBACK(error, "Failed to create pipeline layout.");

    return failure;
  }

  VkGraphicsPipelineCreateInfo pipelineInfo{};

  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages = shaderStages;
  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState = &viewportState;
  pipelineInfo.pRasterizationState = &rasterizer;
  pipelineInfo.pMultisampleState = &multisampling;
  pipelineInfo.pDepthStencilState = nullptr; // Optional
  pipelineInfo.pColorBlendState = &colorBlending;
  pipelineInfo.pDynamicState = nullptr; // Optional
  pipelineInfo.layout = mPipelineLayout;
  pipelineInfo.renderPass = renderPass.getVkRenderPass();
  pipelineInfo.subpass = 0;
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
  pipelineInfo.basePipelineIndex  = -1; // Optional

  result = vkCreateGraphicsPipelines(vkDevice,
                                     VK_NULL_HANDLE,
                                     1,
                                     &pipelineInfo,
                                     nullptr,
                                     &mPipeline);

  if(result not_eq VK_SUCCESS)
  {
    DEBUG_CALLBACK(error,
                   "Failed to create graphics pipeline.",
                   vkCreateGraphicsPipelines);

    return failure;
  }

  return success;
}

void
so::vk::Pipeline::destroyMembers()
{
  VkDevice device(mDevice->getVkDevice());

  if(device not_eq VK_NULL_HANDLE)
  {
    if(mPipeline not_eq VK_NULL_HANDLE)
    { 
      vkDestroyPipeline(device, mPipeline, nullptr);
    }

    if(mPipelineLayout not_eq VK_NULL_HANDLE)
    {
      vkDestroyPipelineLayout(device, mPipelineLayout, nullptr);
    }
  }
}

