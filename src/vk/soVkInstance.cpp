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

#include "soVkInstance.hpp"

#include "cxx/soDebugCallback.hpp"
#include "cxx/soDefinitions.hpp"
#include "cxx/soMemory.hpp"
#include "cxx/soSpan.hpp"

#include <cstring>
#include <iostream>

std::array<char const*, 1> const VALIDATION_LAYERS
  { "VK_LAYER_LUNARG_standard_validation" };

std::shared_ptr<so::vk::Instance> const
so::vk::Instance::getSharedPtrNullInstance()
{
  static std::shared_ptr<so::vk::Instance> 
    sharedPtrNullInstance{ std::make_shared<so::vk::Instance>() };

  return sharedPtrNullInstance;
}

so::vk::Instance::Instance() : mInstance(VK_NULL_HANDLE) {}

so::vk::Instance::~Instance() noexcept
{
  if(mInstance not_eq VK_NULL_HANDLE)
  { 
    vkDestroyInstance(mInstance, nullptr);
  }
}

so::return_t
so::vk::Instance::initialize(std::string const& applicationName,
                             uint32_t    const  applicationVersion,
                             Extensions  const& additionalExtensions)
{
  if(ENABLE_VALIDATION_LAYERS and not checkValidationLayerSupport())
  {
    std::string message{ ": Validation layers requested, but not available"  };

    PREPEND_FUNCTION_SIG_TO_STRING(message);

    executeDebugCallback(error, message);

    return failure;
  }

  std::unique_ptr<VkApplicationInfo> 
    appInfo{ make_unique<VkApplicationInfo>() };

  appInfo->sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo->pApplicationName    = applicationName.c_str();
  appInfo->applicationVersion  = applicationVersion;
  appInfo->pEngineName         = "SolusEngine (Vulkan backend)";
  appInfo->engineVersion       = 1u << 22u;
  appInfo->apiVersion          = 1u << 22u;

  VkInstanceCreateInfo createInfo{ };

  createInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = appInfo.get();

  auto extensions{ getRequiredExtensions() };

  extensions.insert(extensions.end(),
                    additionalExtensions.begin(),
                    additionalExtensions.end());


  executeDebugCallback(info, "Requested extensions:");

  for(auto extension : extensions)
  {
    executeDebugCallback(infoItem, extension);
  }

  createInfo.enabledExtensionCount   =
    static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  if(ENABLE_VALIDATION_LAYERS)
  {
    createInfo.enabledLayerCount   =
      static_cast<uint32_t>(VALIDATION_LAYERS.size());
    createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
  } else
  {
    createInfo.enabledLayerCount   = 0;
  }

  if(vkCreateInstance(&createInfo, nullptr, &mInstance) not_eq VK_SUCCESS)
  { 
    std::string message{ ": Failed to create instance" };

    PREPEND_FUNCTION_SIG_TO_STRING(message);

    executeDebugCallback(error, message);

    return failure;
  }

  return success;
}

bool
so::vk::Instance::checkValidationLayerSupport()
{
  uint32_t layerCount;

  // TODO(BenSolus): Handle enumerateInstanceLayerProperties output

  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);

  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  executeDebugCallback(info, "Available Vulkan layers:");
 
  for(auto const& layerProperties : availableLayers)
  {
    executeDebugCallback(infoItem, layerProperties.layerName);
  }

  for(char const* layerName : VALIDATION_LAYERS)
  {
    bool layerFound{ false };

    for(auto const& layerProperties : availableLayers)
    {
      Span<char const> otherLayerName{ layerProperties.layerName };

      if(std::strcmp(layerName, otherLayerName) is_eq 0)
      {
        layerFound = true;
        break;
      }
    }

    if(not layerFound)
    {
      return false; 
    }
  }

  return true;
}

std::vector<char const*>
so::vk::Instance::getRequiredExtensions()
{
  std::vector<char const*> extensions;

  if(ENABLE_VALIDATION_LAYERS)
  {
    extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
  }

  return extensions;
}

