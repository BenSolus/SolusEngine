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
 *  @file      soVkLogicalDevice.hpp
 *  @author    Bennet Carstensen
 *  @date      2017
 *  @copyright Copyright (c) 2017 Bennet Carstensen
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

#include <soVkPhysicalDevice.hpp>
#include <soVkSurface.hpp>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

namespace so {
namespace vk {
    
class
LogicalDevice : public PhysicalDevice,
                public std::enable_shared_from_this<LogicalDevice>
{
  public:
    static std::shared_ptr<LogicalDevice> const SHARED_PTR_NULL_LOGICAL_DEVICE;

    LogicalDevice();

    LogicalDevice(SharedPtrInstance const& device, Surface const& surface);

    LogicalDevice(LogicalDevice const& other) = delete;

    LogicalDevice(LogicalDevice&& other) = delete;

    ~LogicalDevice() noexcept override;

    LogicalDevice& operator=(LogicalDevice const& other) = delete;

    LogicalDevice&
    operator=(LogicalDevice&& other) noexcept;

    inline auto getVkDevice() { return mDevice; }

    inline auto getGraphicsVkQueue() { return mGraphicsQueue; }

    inline auto getPresentVkQueue() { return mPresentQueue; }

  private:
    VkDevice                mDevice;
    VkQueue                 mGraphicsQueue;
    VkQueue                 mPresentQueue;

    void
    destroyMembers();
};

using SharedPtrLogicalDevice = std::shared_ptr<so::vk::LogicalDevice>;

} // namespace vk
} //namespace so

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop // ignored "-Wnon-virtual-dtor"
#endif

