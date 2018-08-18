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

#include <soVulkan.hpp>

so::Symbol<VkResult,
           VkPhysicalDevice,
           VkDeviceCreateInfo const*,
           VkAllocationCallbacks const*,
           VkDevice*>
so::vk::createDevice;

so::Symbol<void, VkDevice, VkAllocationCallbacks const*>
so::vk::destroyDevice;

so::Symbol<VkResult,
           VkPhysicalDevice,
           char const*,
           uint32_t*,
           VkExtensionProperties*>
so::vk::enumerateDeviceExtensionProperties;

so::Symbol<VkResult, uint32_t*, VkLayerProperties*>
so::vk::enumerateInstanceLayerProperties;

so::Symbol<VkResult, VkInstance, uint32_t*, VkPhysicalDevice*>
so::vk::enumeratePhysicalDevices;

so::Symbol<void, VkDevice, uint32_t, uint32_t, VkQueue*>
so::vk::getDeviceQueue;

so::Symbol<PFN_vkVoidFunction, VkInstance, char const*>
so::vk::getInstanceProcAddr;

so::Symbol<void, VkPhysicalDevice, VkPhysicalDeviceFeatures*>
so::vk::getPhysicalDeviceFeatures;

so::Symbol<void, VkPhysicalDevice, uint32_t*, VkQueueFamilyProperties*>
so::vk::getPhysicalDeviceQueueFamilyProperties;

so::Symbol<VkResult, VkPhysicalDevice, VkSurfaceKHR, VkSurfaceCapabilitiesKHR*>
so::vk::getPhysicalDeviceSurfaceCapabilitiesKHR;

so::Symbol<VkResult,
           VkPhysicalDevice,
           VkSurfaceKHR,
           uint32_t*,
           VkSurfaceFormatKHR*>
so::vk::getPhysicalDeviceSurfaceFormatsKHR;

so::Symbol<VkResult,
           VkPhysicalDevice,
           VkSurfaceKHR,
           uint32_t*,
           VkPresentModeKHR*>
so::vk::getPhysicalDeviceSurfacePresentModesKHR;

so::Symbol<VkResult, VkPhysicalDevice, uint32_t, VkSurfaceKHR, VkBool32*>
so::vk::getPhysicalDeviceSurfaceSupportKHR;

so::Symbol<VkResult, VkDevice, VkSwapchainKHR, uint32_t*, VkImage*>
so::vk::getSwapchainImagesKHR;

so::Symbol<VkResult,
           VkDevice,
           VkImageViewCreateInfo const*,
           VkAllocationCallbacks const*,
           VkImageView*>
so::vk::createImageView;

so::Symbol<void, VkDevice, VkImageView, VkAllocationCallbacks const*>
so::vk::destroyImageView;

so::Symbol<VkResult,
           VkInstanceCreateInfo*,
           VkApplicationInfo*,
           VkInstance*>
so::vk::createInstance;

so::Symbol<void, VkInstance, VkAllocationCallbacks const*>
so::vk::destroyInstance;

so::Symbol<void, VkDevice, VkPipeline, VkAllocationCallbacks const*>
so::vk::destroyPipeline;

so::Symbol<void, VkDevice, VkPipelineLayout, VkAllocationCallbacks const*>
so::vk::destroyPipelineLayout;

so::Symbol<VkResult,
           VkDevice,
           VkShaderModuleCreateInfo const*,
           VkAllocationCallbacks const*,
           VkShaderModule*>
so::vk::createShaderModule;

so::Symbol<void, VkDevice, VkShaderModule, VkAllocationCallbacks const*>
so::vk::destroyShaderModule;

so::Symbol<void, VkInstance, VkSurfaceKHR, VkAllocationCallbacks const*>
so::vk::destroySurfaceKHR;

so::Symbol<VkResult,
           VkDevice,
           VkSwapchainCreateInfoKHR const*,
           VkAllocationCallbacks const*,
           VkSwapchainKHR*>
so::vk::createSwapchainKHR;

so::Symbol<void, VkDevice, VkSwapchainKHR, VkAllocationCallbacks const*>
so::vk::destroySwapchainKHR;

so::DynamicLibrary const so::VulkanLibrary
#if defined(_MSC_VER) || defined(__MINGW32__)
      
#else
  ("libvulkan.so",
#endif
   std::make_pair("vkCreateDevice",
                  std::ref(so::vk::createDevice)),
   std::make_pair("vkDestroyDevice",
                  std::ref(so::vk::destroyDevice)),
   std::make_pair("vkEnumerateDeviceExtensionProperties",
                  std::ref(so::vk::enumerateDeviceExtensionProperties)),
   std::make_pair("vkEnumerateInstanceLayerProperties",
                  std::ref(vk::enumerateInstanceLayerProperties)),
   std::make_pair("vkEnumeratePhysicalDevices",
                  std::ref(so::vk::enumeratePhysicalDevices)),
   std::make_pair("vkGetDeviceQueue",
                  std::ref(so::vk::getDeviceQueue)),
   std::make_pair("vkGetInstanceProcAddr",
                  std::ref(so::vk::getInstanceProcAddr)),   
   std::make_pair("vkGetPhysicalDeviceFeatures",
                  std::ref(so::vk::getPhysicalDeviceFeatures)),
   std::make_pair("vkGetPhysicalDeviceQueueFamilyProperties",
                  std::ref(so::vk::getPhysicalDeviceQueueFamilyProperties)),
   std::make_pair("vkGetPhysicalDeviceSurfaceCapabilitiesKHR",
                  std::ref(so::vk::getPhysicalDeviceSurfaceCapabilitiesKHR)),
   std::make_pair("vkGetPhysicalDeviceSurfaceFormatsKHR",
                  std::ref(so::vk::getPhysicalDeviceSurfaceFormatsKHR)), 
   std::make_pair("vkGetPhysicalDeviceSurfacePresentModesKHR",
                  std::ref(so::vk::getPhysicalDeviceSurfacePresentModesKHR)),   
   std::make_pair("vkGetPhysicalDeviceSurfaceSupportKHR",
                  std::ref(so::vk::getPhysicalDeviceSurfaceSupportKHR)),
   std::make_pair("vkGetSwapchainImagesKHR",
                  std::ref(so::vk::getSwapchainImagesKHR)), 
   std::make_pair("vkCreateImageView",
                  std::ref(so::vk::createImageView)),
   std::make_pair("vkDestroyImageView",
                  std::ref(so::vk::destroyImageView)),
   std::make_pair("vkCreateInstance",
                  std::ref(so::vk::createInstance)),
   std::make_pair("vkDestroyInstance",
                  std::ref(so::vk::destroyInstance)),
   std::make_pair("vkDestroyPipeline",
                  std::ref(so::vk::destroyPipeline)),
   std::make_pair("vkDestroyPipelineLayout",
                  std::ref(so::vk::destroyPipelineLayout)),
   std::make_pair("vkCreateShaderModule",
                  std::ref(so::vk::createShaderModule)),
   std::make_pair("vkDestroyShaderModule",
                  std::ref(so::vk::destroyShaderModule)),
   std::make_pair("vkDestroySurfaceKHR",
                  std::ref(so::vk::destroySurfaceKHR)),
   std::make_pair("vkCreateSwapchainKHR",
                  std::ref(so::vk::createSwapchainKHR)),
   std::make_pair("vkDestroySwapchainKHR",
                  std::ref(so::vk::destroySwapchainKHR)));

std::string
std::to_string(VkResult result)
{
  switch(result)
  {
    case VK_SUCCESS :
      return "VK_SUCCESS";
    case VK_NOT_READY :
      return "VK_NOT_READY";
    case VK_TIMEOUT :
      return "VK_TIMEOUT";
    case VK_EVENT_SET :
      return "VK_EVENT_SET";
    case VK_EVENT_RESET :
      return "VK_EVENT_RESET";
    case VK_INCOMPLETE :
      return "VK_INCOMPLETE";
    case VK_ERROR_OUT_OF_HOST_MEMORY :
      return "VK_ERROR_OUT_OF_HOST_MEMORY";
    case VK_ERROR_OUT_OF_DEVICE_MEMORY :
      return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
    case VK_ERROR_INITIALIZATION_FAILED :
      return "VK_ERROR_INITIALIZATION_FAILED";
    case VK_ERROR_DEVICE_LOST :
      return "VK_ERROR_DEVICE_LOST";
    case VK_ERROR_MEMORY_MAP_FAILED :
      return "VK_ERROR_MEMORY_MAP_FAILED";
    case VK_ERROR_LAYER_NOT_PRESENT :
      return "VK_ERROR_LAYER_NOT_PRESENT";
    case VK_ERROR_EXTENSION_NOT_PRESENT :
      return "VK_ERROR_EXTENSION_NOT_PRESENT";
    case VK_ERROR_FEATURE_NOT_PRESENT :
      return "VK_ERROR_FEATURE_NOT_PRESENT";
    case VK_ERROR_INCOMPATIBLE_DRIVER :
      return "VK_ERROR_INCOMPATIBLE_DRIVER";
    case VK_ERROR_TOO_MANY_OBJECTS :
      return "VK_ERROR_TOO_MANY_OBJECTS";
    case VK_ERROR_FORMAT_NOT_SUPPORTED :
      return "VK_ERROR_FORMAT_NOT_SUPPORTED";
    case VK_ERROR_FRAGMENTED_POOL :
      return "VK_ERROR_FRAGMENTED_POOL";
    case VK_ERROR_OUT_OF_POOL_MEMORY :
      return "VK_ERROR_OUT_OF_POOL_MEMORY";
    case VK_ERROR_INVALID_EXTERNAL_HANDLE :
      return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
    case VK_ERROR_SURFACE_LOST_KHR :
      return "VK_ERROR_SURFACE_LOST_KHR";
    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR :
      return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
    case VK_SUBOPTIMAL_KHR :
      return "VK_SUBOPTIMAL_KHR";
    case VK_ERROR_OUT_OF_DATE_KHR :
      return "VK_ERROR_OUT_OF_DATE_KHR";
    case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR :
      return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
    case VK_ERROR_VALIDATION_FAILED_EXT :
      return "VK_ERROR_VALIDATION_FAILED_EXT"; 
    case VK_ERROR_INVALID_SHADER_NV :
      return "VK_ERROR_INVALID_SHADER_NV";
    case VK_ERROR_FRAGMENTATION_EXT :
      return "VK_ERROR_FRAGMENTATION_EXT";
    case VK_ERROR_NOT_PERMITTED_EXT :
      return "VK_ERROR_NOT_PERMITTED_EXT";
    case VK_RESULT_MAX_ENUM         :
    case VK_RESULT_RANGE_SIZE       :
      break;
  }

  return "VK_ERROR_UNKNOWN";
}
