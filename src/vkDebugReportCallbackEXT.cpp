/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/* Header file with corresponding declarations */
#include <vkDebugReportCallbackEXT.hpp>

/* Libcc */
#include <ccException.hpp>

#include <iostream>

static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugReportFlagsEXT      flags,
              VkDebugReportObjectTypeEXT objType,
              uint64_t                   obj,
              size_t                     location,
              int32_t                    code,
              const char*                layerPrefix,
              const char*                msg,
              void*                      userData);

/******************************************************************************/
/* Constructors and Destructors                                               */
/******************************************************************************/

vk::eng::DebugReportCallbackEXT::DebugReportCallbackEXT()
  : mCallback(nullptr), mInstance(SHARED_PTR_NULL_INSTANCE)
{}

vk::eng::DebugReportCallbackEXT::DebugReportCallbackEXT
  (SharedPtrInstance const& instance)
  : mCallback(nullptr), mInstance(instance)
{
  if(!ENABLE_VALIDATION_LAYERS)
    return;

  VkDebugReportCallbackCreateInfoEXT createInfo = {};

  createInfo.sType =
    VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
  createInfo.flags =
    VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
  createInfo.pfnCallback = debugCallback;

  auto func(reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(
              vkGetInstanceProcAddr(instance->getVkInstance(),
                                    "vkCreateDebugReportCallbackEXT")));

  if((func != nullptr)
     && (func(instance->getVkInstance(), &createInfo, nullptr, &mCallback)
         != VK_SUCCESS))
  {
    cc::throw_with_nested<std::runtime_error>("failed to set up debug "
                                              "callback!",
                                              PRETTY_FUNCTION_SIG);
  }
}

vk::eng::DebugReportCallbackEXT::~DebugReportCallbackEXT() noexcept
{
  VkInstance instance(mInstance->getVkInstance());

  if((mCallback != nullptr) && (instance != VK_NULL_HANDLE))
  {
    auto func(reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>
                (vkGetInstanceProcAddr(instance,
                                       "vkDestroyDebugReportCallbackEXT")));

    if(func != nullptr)
      func(instance, mCallback, nullptr);
  }
}

/******************************************************************************/
/* Source of static functions                                                 */
/******************************************************************************/

VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugReportFlagsEXT      flags,
              VkDebugReportObjectTypeEXT objType,
              uint64_t                   obj,
              size_t                     location,
              int32_t                    code,
              const char*                layerPrefix,
              const char*                msg,
              void*                      userData)
{
  (void) flags;
  (void) objType;
  (void) obj;
  (void) location;
  (void) code;
  (void) layerPrefix;
  (void) userData;

  std::cerr << "validation layer: " << msg << std::endl;

  return VK_FALSE;
}
