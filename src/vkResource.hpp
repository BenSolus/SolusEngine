/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkResource.hpp
 *  @author    Bennet Carstensen
 *  @date      2017
 *  @copyright Copyright (c) 2017 Bennet Carstensen
 *
 *             This Source Code Form is subject to the terms of the
 *             Mozilla Public License, v. 2.0. If a copy of the MPL was
 *             not distributed with this file, You can obtain one at
 *             http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <vkLogicalDevice.hpp>

#include <ccException.hpp>

namespace vk
{
  namespace eng
  {
    template <typename T>
    class
    Resource
    {
      public:
        Resource()
          : mResource(VK_NULL_HANDLE),
            mDeviceMemory(VK_NULL_HANDLE),
            mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE)
        {}

        Resource(SharedPtrLogicalDevice const& device)
          : mResource(VK_NULL_HANDLE),
            mDeviceMemory(VK_NULL_HANDLE),
            mDevice(device)
        {}

        Resource(Resource const& other) = delete;

        Resource(Resource&& other) = delete;

        virtual ~Resource() noexcept = 0;

        Resource&
        operator=(Resource const& other) = delete;

        Resource&
        operator=(Resource&& other) noexcept
        {
          Resource::destroyMembers();

          mResource     = other.mResource;
          mDeviceMemory = other.mDeviceMemory;
          mDevice       = other.mDevice;

          other.mResource     = VK_NULL_HANDLE;
          other.mDeviceMemory = VK_NULL_HANDLE;
          other.mDevice       = SHARED_PTR_NULL_LOGICAL_DEVICE;

          return *this;
        }

        inline T getResource() { return mResource; }

        inline VkDeviceMemory getVkDeviceMemory() { return mDeviceMemory; }

      protected:
        T                      mResource;
        VkDeviceMemory         mDeviceMemory;

        SharedPtrLogicalDevice mDevice;

        uint32_t
        findMemoryType(uint32_t              typeFilter,
                       VkMemoryPropertyFlags properties)
        {
          VkPhysicalDeviceMemoryProperties memProperties;

          vkGetPhysicalDeviceMemoryProperties(mDevice->getVkPhysicalDevice(),
                                              &memProperties);

          for(uint32_t i(0); i < memProperties.memoryTypeCount; ++i)
          {
            if((typeFilter & (1 << i)) && ((memProperties.memoryTypes[i].propertyFlags & properties) == properties))
            {
              return i;
            }
          }

          cc::throw_with_nested<std::runtime_error>("failed to find suitable "
                                                    "memory type!",
                                                    PRETTY_FUNCTION_SIG);

          // Just to silence the compiler
          return std::numeric_limits<uint32_t>::max();
        }

        VkCommandBuffer
        beginSingleTimeCommands(VkCommandPool commandPool)
        {
          VkCommandBufferAllocateInfo allocInfo = {};

          allocInfo.sType              =
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
          allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
          allocInfo.commandPool        = commandPool;
          allocInfo.commandBufferCount = 1;

          VkCommandBuffer commandBuffer;

          vkAllocateCommandBuffers(mDevice->getVkDevice(),
                                   &allocInfo,
                                   &commandBuffer);

          VkCommandBufferBeginInfo beginInfo = {};

          beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
          beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

          vkBeginCommandBuffer(commandBuffer, &beginInfo);

          return commandBuffer;
        }

        void
        endSingleTimeCommands(VkCommandPool   commandPool,
                              VkCommandBuffer commandBuffer)
        {
          vkEndCommandBuffer(commandBuffer);

          VkSubmitInfo submitInfo = {};

          submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
          submitInfo.commandBufferCount = 1;
          submitInfo.pCommandBuffers    = &commandBuffer;

          vkQueueSubmit(mDevice->getGraphicsVkQueue(),
                        1,
                        &submitInfo,
                        VK_NULL_HANDLE);

          vkQueueWaitIdle(mDevice->getGraphicsVkQueue());

          vkFreeCommandBuffers(mDevice->getVkDevice(),
                               commandPool,
                               1,
                               &commandBuffer);
        }

        virtual void destroyMembers() = 0;
    };

    template <typename T>
    Resource<T>::~Resource() noexcept { Resource<T>::destroyMembers(); }

    template <typename T>
    void
    Resource<T>::destroyMembers()
    {
      VkDevice device(mDevice->getVkDevice());

      if((device != VK_NULL_HANDLE) && (mDeviceMemory != VK_NULL_HANDLE))
        vkFreeMemory(device, mDeviceMemory, nullptr);
    }
  } // namespace eng
} // namespace vk

