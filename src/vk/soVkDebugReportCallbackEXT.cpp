/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/* Header file with corresponding declarations */
#include <vk/soVkDebugReportCallbackEXT.hpp>

#include <utils/err/soException.hpp>

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

so::vk::DebugReportCallbackEXT::DebugReportCallbackEXT()
  : mCallback(nullptr), mInstance(SHARED_PTR_NULL_INSTANCE)
{}

so::vk::DebugReportCallbackEXT::DebugReportCallbackEXT
  (SharedPtrInstance const& instance)
  : mCallback(nullptr), mInstance(instance)
{
  if(!ENABLE_VALIDATION_LAYERS)
    return;

  VkDebugReportCallbackCreateInfoEXT createInfo({});

  createInfo.sType       =
    VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
  createInfo.flags       =
    VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
  createInfo.pfnCallback = debugCallback;

  auto func(reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>
    (vkGetInstanceProcAddr(instance->getVkInstance(),
                           "vkCreateDebugReportCallbackEXT")));

  if(func not_eq nullptr)
  {
    VkResult const result(func(instance->getVkInstance(),
                               &createInfo,
                               nullptr,
                               &mCallback));

    if (result not_eq VK_SUCCESS)
      throw utils::err::Exception<std::runtime_error>("failed to set up debug "
                                                      "callback!",
                                                      PRETTY_FUNCTION_SIG);
  }
}

so::vk::DebugReportCallbackEXT::~DebugReportCallbackEXT() noexcept
{
  deleteMembers();
}

void
so::vk::DebugReportCallbackEXT::deleteMembers()
{
  VkInstance const instance(mInstance->getVkInstance());

  if((mCallback not_eq nullptr) and (instance not_eq VK_NULL_HANDLE))
  {
    auto func(reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>
      (vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT")));

    if(func not_eq nullptr)
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
