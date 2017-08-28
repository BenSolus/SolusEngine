/*
 * Copyright (C) 2017 by Bennet Carstensen
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

/**
 *  @file      soVkCommandPool.hpp
 *  @author    Bennet Carstensen
 *  @date      2017
 *  @copyright Copyright (c) 2017 Bennet Carstensen
 *
 *             Permission is hereby granted, free of charge, to any person
 *             obtaining a copy of this software and associated documentation
 *             files (the "Software"), to deal in the Software without
 *             restriction, including without limitation the rights to use,
 *             copy, modify, merge, publish, distribute, sublicense, and/or sell
 *             copies of the Software, and to permit persons to whom the
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

#include <vk/soVkLogicalDevice.hpp>
#include <vk/soVkSurface.hpp>

namespace so
{
  namespace vk
  {
    class
    CommandPool : public std::enable_shared_from_this<CommandPool>
    {
      public:
        CommandPool();

        CommandPool(SharedPtrLogicalDevice const& device,
                    Surface&                      surface);

        CommandPool(SharedPtrLogicalDevice const& device,
                    VkCommandPool                 commandPool);

        CommandPool(CommandPool& other) = delete;

        CommandPool(CommandPool&& other) = delete;

        ~CommandPool() noexcept;

        CommandPool&
        operator=(CommandPool const& other) = delete;

        CommandPool&
        operator=(CommandPool&& other) noexcept
        {
          destroyMembers();

          mCommandPool = other.mCommandPool;
          mDevice      = other.mDevice;

          other.mCommandPool = VK_NULL_HANDLE;
          other.mDevice      = SHARED_PTR_NULL_LOGICAL_DEVICE;

          return *this;
        }

        inline auto getVkCommandPool() { return mCommandPool; }

      private:
        VkCommandPool          mCommandPool;

        SharedPtrLogicalDevice mDevice;

        void
        destroyMembers();
    };

    using SharedPtrCommandPool = std::shared_ptr<CommandPool>;
  } // namespace vk
} // namespace so

/**
 *
 * @todo When using default constructor, an additional std::shared_ptr is
 *       generated from @ref SHARED_PTR_NULL_LOGICAL_DEVICE as member @p
 *       mDevice. However, SHARED_PTR_NULL_LOGICAL_DEVICE seems to get destroyed
 *       before SHARED_PTR_NULL_COMMAND_POOL. Current workaround is defining
 *       an additional constructor which allows setting all member variables,
 *       defining another "invalid" instance of fcm::vk::SharedPtrLogicalDevice
 *       identical to SHARED_PTR_NULL_LOGICAL_DEVICE and setting the other
 *       member identical to the default constructor. Needless to say that
 *       that defining an additional constructor and creating an additional
 *       instance is kind of ugly...
 */
extern so::vk::SharedPtrCommandPool const SHARED_PTR_NULL_COMMAND_POOL;