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
 *  @file      soVkPipeline.hpp
 *  @author    Bennet Carstensen
 *  @date      2017
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

#include <soVkLogicalDevice.hpp>

namespace so {
namespace vk {
    
class
Pipeline
{
  public:
    Pipeline();

    Pipeline(SharedPtrLogicalDevice const& device,
             VkExtent2D                    swapChainExtent,
             VkRenderPass                  renderPass,
             VkDescriptorSetLayout&        descriptorSetLayout);

    Pipeline(Pipeline const& other) = delete;

    Pipeline(Pipeline&& other) = delete;

    ~Pipeline() noexcept;

    Pipeline&
    operator=(Pipeline const& other) = delete;

    Pipeline&
    operator=(Pipeline&& other) noexcept
    {
      if(this == &other)
        return *this;

      destroyMembers();

      mPipeline       = other.mPipeline;
      mPipelineLayout = other.mPipelineLayout;
      mDevice         = other.mDevice;

      other.mPipeline       = VK_NULL_HANDLE;
      other.mPipelineLayout = VK_NULL_HANDLE;
      other.mDevice         = LogicalDevice::SHARED_PTR_NULL_LOGICAL_DEVICE;

      return *this;
    }

    inline VkPipeline getVkPipeline() { return mPipeline; }
 
    inline VkPipelineLayout getVkPipelineLayout() { return mPipelineLayout; }

  private:
    VkPipeline                mPipeline;
    VkPipelineLayout          mPipelineLayout;

    SharedPtrLogicalDevice    mDevice;

    void
    destroyMembers();
};
  
} // namespace vk
} // namespace so
