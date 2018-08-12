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
                  std::reference_wrapper<Symbol<VkResult,
                                                VkPhysicalDevice,
                                                VkDeviceCreateInfo const*,
                                                VkAllocationCallbacks const*,
                                                VkDevice*>>
                    (so::vk::createDevice)),
   std::make_pair("vkDestroyDevice",
                  std::reference_wrapper<Symbol<void,
                                                VkDevice,
                                                VkAllocationCallbacks const*>>
                    (so::vk::destroyDevice)),
   std::make_pair("vkEnumerateDeviceExtensionProperties",
                  std::reference_wrapper<Symbol<VkResult,
                                                VkPhysicalDevice,
                                                char const*,
                                                uint32_t*,
                                                VkExtensionProperties*>>
                    (so::vk::enumerateDeviceExtensionProperties)),
   std::make_pair("vkEnumerateInstanceLayerProperties",
                  std::reference_wrapper<Symbol<VkResult,
                                                uint32_t*,
                                                VkLayerProperties*>>
                    (so::vk::enumerateInstanceLayerProperties)),
   std::make_pair("vkEnumeratePhysicalDevices",
                  std::reference_wrapper<Symbol<VkResult,
                                                VkInstance,
                                                uint32_t*,
                                                VkPhysicalDevice*>>
                    (so::vk::enumeratePhysicalDevices)),
   std::make_pair("vkGetDeviceQueue",
                  std::reference_wrapper<Symbol<void,
                                                VkDevice,
                                                uint32_t,
                                                uint32_t,
                                                VkQueue*>>
                    (so::vk::getDeviceQueue)),
   std::make_pair("vkGetInstanceProcAddr",
                  std::reference_wrapper<Symbol<PFN_vkVoidFunction,
                                                VkInstance,
                                                char const*>>
                    (so::vk::getInstanceProcAddr)),   
   std::make_pair("vkGetPhysicalDeviceFeatures",
                  std::reference_wrapper<Symbol<void,
                                                VkPhysicalDevice,
                                                VkPhysicalDeviceFeatures*>>
                    (so::vk::getPhysicalDeviceFeatures)),
   std::make_pair("vkGetPhysicalDeviceQueueFamilyProperties",
                  std::reference_wrapper<Symbol<void,
                                                VkPhysicalDevice,
                                                uint32_t*,
                                                VkQueueFamilyProperties*>>
                    (so::vk::getPhysicalDeviceQueueFamilyProperties)),
   std::make_pair("vkGetPhysicalDeviceSurfaceCapabilitiesKHR",
                  std::reference_wrapper<Symbol<VkResult,
                                                VkPhysicalDevice,
                                                VkSurfaceKHR,
                                                VkSurfaceCapabilitiesKHR*>>
                    (so::vk::getPhysicalDeviceSurfaceCapabilitiesKHR)),
   std::make_pair("vkGetPhysicalDeviceSurfaceFormatsKHR",
                  std::reference_wrapper<Symbol<VkResult,
                                                VkPhysicalDevice,
                                                VkSurfaceKHR,
                                                uint32_t*,
                                                VkSurfaceFormatKHR*>>
                    (so::vk::getPhysicalDeviceSurfaceFormatsKHR)), 
   std::make_pair("vkGetPhysicalDeviceSurfacePresentModesKHR",
                  std::reference_wrapper<Symbol<VkResult,
                                                VkPhysicalDevice,
                                                VkSurfaceKHR,
                                                uint32_t*,
                                                VkPresentModeKHR*>>
                    (so::vk::getPhysicalDeviceSurfacePresentModesKHR)),   
   std::make_pair("vkGetPhysicalDeviceSurfaceSupportKHR",
                  std::reference_wrapper<Symbol<VkResult,
                                                VkPhysicalDevice,
                                                uint32_t,
                                                VkSurfaceKHR,
                                                VkBool32*>>
                    (so::vk::getPhysicalDeviceSurfaceSupportKHR)),
   std::make_pair("vkGetSwapchainImagesKHR",
                  std::reference_wrapper<Symbol<VkResult,
                                                VkDevice,
                                                VkSwapchainKHR,
                                                uint32_t*,
                                                VkImage*>>
                    (so::vk::getSwapchainImagesKHR)), 
   std::make_pair("vkCreateImageView",
                  std::reference_wrapper<Symbol<VkResult,
                                                VkDevice,
                                                VkImageViewCreateInfo const*,
                                                VkAllocationCallbacks const*,
                                                VkImageView*>>
                    (so::vk::createImageView)),
   std::make_pair("vkDestroyImageView",
                  std::reference_wrapper<Symbol<void,
                                                VkDevice,
                                                VkImageView,
                                                VkAllocationCallbacks const*>>
                    (so::vk::destroyImageView)),
   std::make_pair("vkCreateInstance",
                  std::reference_wrapper<Symbol<VkResult,
                                                VkInstanceCreateInfo*,
                                                VkApplicationInfo*,
                                                VkInstance*>>
                    (so::vk::createInstance)),
   std::make_pair("vkDestroyInstance",
                  std::reference_wrapper<Symbol<void,
                                                VkInstance,
                                                VkAllocationCallbacks const*>>
                    (so::vk::destroyInstance)),
   std::make_pair("vkDestroySurfaceKHR",
                  std::reference_wrapper<Symbol<void,
                                                VkInstance,
                                                VkSurfaceKHR,
                                                VkAllocationCallbacks const*>>
                    (so::vk::destroySurfaceKHR)),
   std::make_pair("vkCreateSwapchainKHR",
                  std::reference_wrapper<Symbol
                                          <VkResult,
                                           VkDevice,
                                           VkSwapchainCreateInfoKHR const*,
                                           VkAllocationCallbacks const*,
                                           VkSwapchainKHR*>>
                    (so::vk::createSwapchainKHR)),
   std::make_pair("vkDestroySwapchainKHR",
                  std::reference_wrapper<Symbol<void,
                                                VkDevice,
                                                VkSwapchainKHR,
                                                VkAllocationCallbacks const*>>
                    (so::vk::destroySwapchainKHR)));

std::string
std::to_string(VkResult result)
{
  switch(result)
  {
    case 0  : return "VK_SUCCESS";
    case 1  : return "VK_NOT_READY";
    case 2  : return "VK_TIMEOUT";
    case 3  : return "VK_EVENT_SET";
    case 4  : return "VK_EVENT_RESET";
    case 5  : return "VK_INCOMPLETE";
    case -1 : return "VK_ERROR_OUT_OF_HOST_MEMORY";
    case -2 : return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
    case -3 : return "VK_ERROR_INITIALIZATION_FAILED";
    case -4 : return "VK_ERROR_DEVICE_LOST";
    case -5 : return "VK_ERROR_MEMORY_MAP_FAILED";
    case -6 : return "VK_ERROR_LAYER_NOT_PRESENT";
    case -7 : return "VK_ERROR_EXTENSION_NOT_PRESENT";
    case -8 : return "VK_ERROR_FEATURE_NOT_PRESENT";
    case -9 : return "VK_ERROR_INCOMPATIBLE_DRIVER";
    case -10 : return "VK_ERROR_TOO_MANY_OBJECTS";
    case -11 : return "VK_ERROR_FORMAT_NOT_SUPPORTED";
    case -12 : return "VK_ERROR_FRAGMENTED_POOL";
    case -1000069000 : return "VK_ERROR_OUT_OF_POOL_MEMORY";
    case -1000072003 : return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
    case -1000000000 : return "VK_ERROR_SURFACE_LOST_KHR";
    case -1000000001 : return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
    case  1000001003 : return "VK_SUBOPTIMAL_KHR";
    case -1000001004 : return "VK_ERROR_OUT_OF_DATE_KHR";
    case -1000003001 : return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
    case -1000011001 : return "VK_ERROR_VALIDATION_FAILED_EXT"; 
    case -1000012000 : return "VK_ERROR_INVALID_SHADER_NV";
    case -1000161000 : return "VK_ERROR_FRAGMENTATION_EXT";
    case VK_ERROR_NOT_PERMITTED_EXT : return "VK_ERROR_NOT_PERMITTED_EXT";
    case VK_RESULT_MAX_ENUM         :
    case VK_RESULT_RANGE_SIZE       : break;
  }

  return "VK_ERROR_UNKNOWN";
}
