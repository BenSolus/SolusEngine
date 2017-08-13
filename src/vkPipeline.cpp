/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vkPipeline.hpp>

#include <vkShaderModule.hpp>
#include <vkVertex.hpp>

#include <ccConstants.hpp>
#include <ccException.hpp>

vk::eng::Pipeline::Pipeline()
  : mPipeline(VK_NULL_HANDLE),
    mPipelineLayout(VK_NULL_HANDLE),
    mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE)
{}

vk::eng::Pipeline::Pipeline(SharedPtrLogicalDevice const& device,
                            VkExtent2D                    swapChainExtent,
                            VkRenderPass                  renderPass,
                            VkDescriptorSetLayout&        descriptorSetLayout)
: mPipeline(VK_NULL_HANDLE),
  mPipelineLayout(VK_NULL_HANDLE),
  mDevice(device)
{
  ShaderModule vertShaderModule, fragShaderModule;

  try
  {
    vertShaderModule = vk::eng::ShaderModule(device,
                                             BIN_DIR +
                                             "/data/shaders/vert.spv");

    fragShaderModule = vk::eng::ShaderModule(device,
                                             BIN_DIR +
                                             "/data/shaders/frag.spv");
  }
  catch(...)
  {
    cc::throw_with_nested<std::runtime_error>("Caught exception:",
                                              PRETTY_FUNCTION_SIG);
  }

  VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};

  vertShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertShaderStageInfo.stage  = VK_SHADER_STAGE_VERTEX_BIT;
  vertShaderStageInfo.module = vertShaderModule.getVkShaderModule();
  vertShaderStageInfo.pName  = "main";

  VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};

  fragShaderStageInfo.sType    = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule.getVkShaderModule();
    fragShaderStageInfo.pName  = "main";

  VkPipelineShaderStageCreateInfo shaderStages[] =
    { vertShaderStageInfo, fragShaderStageInfo };

  VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};

  auto bindingDescription(Vertex::getBindingDescription());
  auto attributeDescriptions(Vertex::getAttributeDescriptions());

  vertexInputInfo.sType =
    VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputInfo.vertexBindingDescriptionCount   = 1;
  vertexInputInfo.vertexAttributeDescriptionCount =
    static_cast<uint32_t>(attributeDescriptions.size());
  vertexInputInfo.pVertexBindingDescriptions      = &bindingDescription;
  vertexInputInfo.pVertexAttributeDescriptions    =
    attributeDescriptions.data();

  VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};

  inputAssembly.sType =
    VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssembly.primitiveRestartEnable = VK_FALSE;

  VkViewport viewport = {};

  viewport.x        = 0.0f;
  viewport.y        = 0.0f;
  viewport.width    = static_cast<float>(swapChainExtent.width);
  viewport.height   = static_cast<float>(swapChainExtent.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  VkRect2D scissor = {};

  scissor.offset = {0, 0};
  scissor.extent = swapChainExtent;

  VkPipelineViewportStateCreateInfo viewportState = {};

  viewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.viewportCount = 1;
  viewportState.pViewports    = &viewport;
  viewportState.scissorCount  = 1;
  viewportState.pScissors     = &scissor;

  VkPipelineRasterizationStateCreateInfo rasterizer = {};

  rasterizer.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable        = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode             = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth               = 1.0f;
  rasterizer.cullMode                = VK_CULL_MODE_BACK_BIT;
  rasterizer.frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  rasterizer.depthBiasEnable         = VK_FALSE;

  VkPipelineMultisampleStateCreateInfo multisampling = {};

  multisampling.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable  = VK_FALSE;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

  VkPipelineDepthStencilStateCreateInfo depthStencil = {};

  depthStencil.sType                 =
    VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  depthStencil.depthTestEnable       = VK_TRUE;
  depthStencil.depthWriteEnable      = VK_TRUE;
  depthStencil.depthCompareOp        = VK_COMPARE_OP_LESS;
  depthStencil.depthBoundsTestEnable = VK_FALSE;
  depthStencil.stencilTestEnable     = VK_FALSE;

  VkPipelineColorBlendAttachmentState colorBlendAttachment = {};

  colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
                                        VK_COLOR_COMPONENT_G_BIT |
                                        VK_COLOR_COMPONENT_B_BIT |
                                        VK_COLOR_COMPONENT_A_BIT;
  colorBlendAttachment.blendEnable    = VK_FALSE;

  VkPipelineColorBlendStateCreateInfo colorBlending = {};

  colorBlending.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlending.logicOpEnable     = VK_FALSE;
  colorBlending.logicOp           = VK_LOGIC_OP_COPY;
  colorBlending.attachmentCount   = 1;
  colorBlending.pAttachments      = &colorBlendAttachment;
  colorBlending.blendConstants[0] = 0.0f;
  colorBlending.blendConstants[1] = 0.0f;
  colorBlending.blendConstants[2] = 0.0f;
  colorBlending.blendConstants[3] = 0.0f;

  VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};

  pipelineLayoutInfo.sType                  =
    VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount         = 1;
  pipelineLayoutInfo.pSetLayouts            = &descriptorSetLayout;

  if(vkCreatePipelineLayout(device->getVkDevice(), &pipelineLayoutInfo, nullptr, &mPipelineLayout) != VK_SUCCESS)
  {
    cc::throw_with_nested<std::runtime_error>("failed to create "
                                              "pipeline layout!",
                                              PRETTY_FUNCTION_SIG);
  }

  VkGraphicsPipelineCreateInfo pipelineInfo = {};

  pipelineInfo.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount          = 2;
  pipelineInfo.pStages             = shaderStages;
  pipelineInfo.pVertexInputState   = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState      = &viewportState;
  pipelineInfo.pRasterizationState = &rasterizer;
  pipelineInfo.pMultisampleState   = &multisampling;
  pipelineInfo.pDepthStencilState  = &depthStencil;
  pipelineInfo.pColorBlendState    = &colorBlending;
  pipelineInfo.pDynamicState       = nullptr;
  pipelineInfo.layout              = mPipelineLayout;
  pipelineInfo.renderPass          = renderPass;
  pipelineInfo.subpass             = 0;

  if(vkCreateGraphicsPipelines(device->getVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &mPipeline) != VK_SUCCESS)
  {
    cc::throw_with_nested<std::runtime_error>("failed to create "
                                              "graphics pipeline!",
                                              PRETTY_FUNCTION_SIG);
  }
}

vk::eng::Pipeline::~Pipeline() noexcept
{
  destroyMembers();
}

void
vk::eng::Pipeline::destroyMembers()
{
  VkDevice device(mDevice->getVkDevice());

  if(device != VK_NULL_HANDLE)
  {
    if(mPipeline != VK_NULL_HANDLE)
      vkDestroyPipeline(device, mPipeline, nullptr);

    if(mPipelineLayout != VK_NULL_HANDLE)
      vkDestroyPipelineLayout(device, mPipelineLayout, nullptr);
  }
}
