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

#include <soVkInstance.hpp>

#include <soDefinitions.hpp>
#include <soException.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstring>

std::vector<char const*> const VALIDATION_LAYERS
  ({ "VK_LAYER_LUNARG_standard_validation" });

so::vk::SharedPtrInstance const so::vk::Instance::SHARED_PTR_NULL_INSTANCE
  (std::make_shared<so::vk::Instance>
    (static_cast<VkInstance>(VK_NULL_HANDLE)));

so::vk::Instance::Instance() : mInstance(VK_NULL_HANDLE) {}

so::vk::Instance::Instance(std::string const& applicationName,
                           uint32_t    const  applicationVersion)
  : Instance()
{
  if(ENABLE_VALIDATION_LAYERS and not checkValidationLayerSupport())
    THROW_EXCEPTION("Validation layers requested, but not vailable!");

  VkApplicationInfo* appInfo(new VkApplicationInfo({}));

  appInfo->sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo->pApplicationName    = applicationName.c_str();
  appInfo->applicationVersion  = applicationVersion;
  appInfo->pEngineName         = "SolusEngine (Vulkan backend)";
  appInfo->engineVersion       = VK_MAKE_VERSION(1, 0, 0);
  appInfo->apiVersion          = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo({});

  createInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = appInfo;

  auto extensions(getRequiredExtensions());

  createInfo.enabledExtensionCount   =
    static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  if(ENABLE_VALIDATION_LAYERS)
  {
    createInfo.enabledLayerCount   =
      static_cast<uint32_t>(VALIDATION_LAYERS.size());
    createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
  } else
    createInfo.enabledLayerCount   = 0;

  if(createInstance(&createInfo, nullptr, &mInstance) not_eq VK_SUCCESS)
    THROW_EXCEPTION("failed to create instance!");

  delete appInfo;
}

so::vk::Instance::~Instance() noexcept
{
  if(mInstance not_eq VK_NULL_HANDLE)
    destroyInstance(mInstance, nullptr);
}

bool
so::vk::Instance::checkValidationLayerSupport()
{
  uint32_t layerCount;

  // TODO: Handle enumerateInstanceLayerProperties output

  enumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);

  enumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  std::cout << "<INFO> Available Vulkan layers:\n";
  for(auto const& layerProperties : availableLayers)
    std::cout << "<INFO>   " << layerProperties.layerName << "\n";

  for(char const* layerName : VALIDATION_LAYERS)
  {
    bool layerFound = false;

    for(auto const& layerProperties : availableLayers)
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

std::vector<char const*>
so::vk::Instance::getRequiredExtensions()
{
  uint32_t glfwExtensionCount(0);

  std::vector<char const*> extensions;

  char const** glfwExtensions(glfwGetRequiredInstanceExtensions
                                (&glfwExtensionCount));

  for(unsigned int i(0); i < glfwExtensionCount; ++i)
    extensions.push_back(glfwExtensions[i]);

  if(ENABLE_VALIDATION_LAYERS)
    extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

  return extensions;
}

