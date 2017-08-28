/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vk/soVkInstance.hpp>

#include <utils/def/soDefinitions.hpp>
#include <utils/err/soException.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

const std::vector<const char*> VALIDATION_LAYERS
  ({ "VK_LAYER_LUNARG_standard_validation" });

so::vk::Instance::Instance()
  : mInstance(VK_NULL_HANDLE)
{
  if(ENABLE_VALIDATION_LAYERS and not checkValidationLayerSupport())
    throw utils::err::Exception<std::runtime_error>("Validation layers "
                                                    "requested, but not "
                                                    "available!",
                                                    PRETTY_FUNCTION_SIG);

  VkApplicationInfo* appInfo(new VkApplicationInfo({}));

  appInfo->sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo->pApplicationName    = "Vulkan Engine";
  appInfo->applicationVersion  = VK_MAKE_VERSION(1, 0, 0);
  appInfo->pEngineName         = "SolusEngine";
  appInfo->engineVersion       = VK_MAKE_VERSION(1, 0, 0);
  appInfo->apiVersion          = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo({});

  createInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = appInfo;

  auto extensions(getRequiredExtensions());

  createInfo.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  if(ENABLE_VALIDATION_LAYERS)
  {
    createInfo.enabledLayerCount   =
      static_cast<uint32_t>(VALIDATION_LAYERS.size());
    createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
  } else
    createInfo.enabledLayerCount   = 0;

  if(vkCreateInstance(&createInfo, nullptr, &mInstance) not_eq VK_SUCCESS)
    throw utils::err::Exception<std::runtime_error>("failed to create "
                                                    "instance!",
                                                    PRETTY_FUNCTION_SIG);

  delete appInfo;
}

so::vk::Instance::~Instance() noexcept
{
  if(mInstance not_eq VK_NULL_HANDLE)
    vkDestroyInstance(mInstance, nullptr);
}

bool
so::vk::Instance::checkValidationLayerSupport()
{
  uint32_t layerCount;

  std::vector<VkLayerProperties> availableLayers;

  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  availableLayers = std::vector<VkLayerProperties>(layerCount);

  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for(const char* layerName : VALIDATION_LAYERS)
  {
    bool layerFound = false;

    for(const auto& layerProperties : availableLayers)
      if(std::strcmp(layerName, layerProperties.layerName) is_eq 0)
      {
        layerFound = true;
        break;
      }

    if(not layerFound)
      return false;
  }

  return true;
}

std::vector<const char*>
so::vk::Instance::getRequiredExtensions()
{
  unsigned int glfwExtensionCount(0);

  std::vector<const char*> extensions;

  const char** glfwExtensions(glfwGetRequiredInstanceExtensions(&glfwExtensionCount));

  for(unsigned int i(0); i < glfwExtensionCount; ++i)
    extensions.push_back(glfwExtensions[i]);

  if(ENABLE_VALIDATION_LAYERS)
    extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

  return extensions;
}

so::vk::SharedPtrInstance const SHARED_PTR_NULL_INSTANCE
  (std::make_shared<so::vk::Instance>
      (static_cast<VkInstance>(VK_NULL_HANDLE)));
