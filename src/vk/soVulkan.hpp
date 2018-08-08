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

/**
 *  @file      vk/soVulkan.hpp
 *  @author    Bennet Carstensen
 *  @date      2018
 *  @copyright Copyright (c) 2017-2018 Bennet Carstensen
 *
 *             Permission is hereby granted, free of charge, to any person
 *             obtaining a copy of this software and associated documentation
 *             files (the "Software"), to deal in the Software without
 *             restriction, including without limitation the rights to use,
 *             copy, modify, merge, publish, distribute, sublicense, and/or
 *             sell copies of the Software, and to permit persons to whom the
 *             Software is furnished to do so, subject to the following
 *             conditions:
 *
 *             The above copyright notice and this permission notice shall be
 *             included in all copies or substantial portions of the Software.
 *
 *             THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *             EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *             OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *             NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *             HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *             WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *             FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *             OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <soDynamicLibrary.hpp>

#include <vulkan/vulkan.hpp>

namespace so {

extern DynamicLibrary const VulkanLibrary;

namespace vk {

extern Symbol<VkResult,
              VkPhysicalDevice,
              VkDeviceCreateInfo const*,
              VkAllocationCallbacks const*,
              VkDevice*>
createDevice;

extern Symbol<void, VkDevice, VkAllocationCallbacks const*>
destroyDevice;

extern Symbol<VkResult,
              VkPhysicalDevice,
              char const*,
              uint32_t*,
              VkExtensionProperties*>
enumerateDeviceExtensionProperties;

extern Symbol<VkResult, uint32_t*, VkLayerProperties*>
enumerateInstanceLayerProperties;

extern Symbol<VkResult, VkInstance, uint32_t*, VkPhysicalDevice*>
enumeratePhysicalDevices;

extern Symbol<VkResult, VkInstanceCreateInfo*, VkApplicationInfo*, VkInstance*>
createInstance;

extern Symbol<void, VkInstance, VkAllocationCallbacks const*>
destroyInstance;

extern Symbol<void, VkDevice, uint32_t, uint32_t, VkQueue*>
getDeviceQueue;

extern Symbol<PFN_vkVoidFunction, VkInstance, char const*>
getInstanceProcAddr;

extern Symbol<void, VkPhysicalDevice, VkPhysicalDeviceFeatures*>
getPhysicalDeviceFeatures;

extern Symbol<void, VkPhysicalDevice, uint32_t*, VkQueueFamilyProperties*>
getPhysicalDeviceQueueFamilyProperties;

extern Symbol<VkResult,
              VkPhysicalDevice,
              VkSurfaceKHR,
              VkSurfaceCapabilitiesKHR*>
getPhysicalDeviceSurfaceCapabilitiesKHR;

extern Symbol<VkResult,
              VkPhysicalDevice,
              VkSurfaceKHR,
              uint32_t*,
              VkSurfaceFormatKHR*>
getPhysicalDeviceSurfaceFormatsKHR;

extern Symbol<VkResult,
              VkPhysicalDevice,
              VkSurfaceKHR,
              uint32_t*,
              VkPresentModeKHR*>
getPhysicalDeviceSurfacePresentModesKHR;

extern Symbol<VkResult,
              VkPhysicalDevice,
              uint32_t,
              VkSurfaceKHR,
              VkBool32*>
getPhysicalDeviceSurfaceSupportKHR;

extern Symbol<void, VkInstance, VkSurfaceKHR, VkAllocationCallbacks const*>
destroySurfaceKHR;

} // namespace vk
} // namespace so

namespace std {

string
to_string(VkResult result);

} // namespace std
 
