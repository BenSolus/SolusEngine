/*
 * Copyright (C) 2017-2018 by Bennet Carstensen
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

#include "soVkDebugReportCallbackEXT.hpp"

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

so::vk::DebugReportCallbackEXT::DebugReportCallbackEXT()
  : mCallback(nullptr), mInstance(Instance::getSharedPtrNullInstance())
{}

so::vk::DebugReportCallbackEXT::~DebugReportCallbackEXT() noexcept
{
  deleteMembers();
}

// so::vk::DebugReportCallbackEXT&
// so::vk::DebugReportCallbackEXT::operator=
//   (DebugReportCallbackEXT&& other) noexcept
// {
//   if(this is_eq &other)
//   {
//     return *this;
//   }

//  deleteMembers();

//   mCallback = other.mCallback;
//   mInstance = other.mInstance;

//   other.mCallback = nullptr;
//   other.mInstance = Instance::getSharedPtrNullInstance();

//   return *this;
// }

so::return_t
so::vk::DebugReportCallbackEXT::initialize(SharedPtrInstance const& instance)
{
  mInstance = instance;

  if(not ENABLE_VALIDATION_LAYERS)
  {
    return success;
  }

  VkDebugReportCallbackCreateInfoEXT createInfo{ };

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

    if(result not_eq VK_SUCCESS)
    {
      return failure;
    }
  }

  return success;
}

void
so::vk::DebugReportCallbackEXT::deleteMembers()
{
  VkInstance instance(mInstance->getVkInstance());

  if((mCallback not_eq nullptr) and (instance not_eq VK_NULL_HANDLE))
  {
    auto func(reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>
      (vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT")));

    if(func not_eq nullptr)
    {
      func(instance, mCallback, nullptr);
    }
  }
}

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

  std::cerr << "<ERROR> validation layer: " << msg << std::endl;

  return VK_FALSE;
}

