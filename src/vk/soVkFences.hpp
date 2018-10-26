/* Copyright (C) 2017-2018 by Bennet Carstensen
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
 *  @file      soVkFences.hpp
 *  @author    Bennet Carstensen
 *  @date      2018
 *  @copyright Copyright (c) 2017-2018 Bennet Carstensen
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

#include "soVkLogicalDevice.hpp"

#include "cxx/soDebugCallback.hpp"

#include <vector>

namespace so {
namespace vk {

template<typename FC = std::vector<VkFence>>
class
Fences
{
    using fence_container = FC;
    using size_type       = typename fence_container::size_type;

//    static_assert
//      (std::is_same<std::result_of<decltype(decltype(&FC::at)(FC, std::size_t))>::type,
//                    VkFence>::value,
//       "Template parameter is not a container of VkFences.");
  public:
    Fences();

    Fences(const Fences& other) = delete;

    Fences(Fences&& other) = delete;

    ~Fences() noexcept;

    Fences&
    operator=(const Fences& other) = delete;

    Fences&
    operator=(Fences&& other) = delete;

    return_t
    initialize(      SharedPtrLogicalDevice device,
               const size_type              numFences);

    VkFence&
    operator[](const index_t idx);

  private:
    fence_container        mFences;

    SharedPtrLogicalDevice mDevice;
}; // class Fences

} // namespace vk
} // namespace so

template<typename FC>
so::vk::Fences<FC>::Fences()
  : mFences(0),
    mDevice(LogicalDevice::getSharedPtrNullDevice())
{}

template<typename FC>
so::vk::Fences<FC>::~Fences() noexcept
{
  VkDevice device{ mDevice->getVkDevice() };

  for(auto& fence : mFences)
  {
    if(fence not_eq VK_NULL_HANDLE)
    {
      vkDestroyFence(device, fence, nullptr);
    }
  }
}

template<typename FC>
so::return_t
so::vk::Fences<FC>::initialize(      SharedPtrLogicalDevice device,
                               const size_type              numFences)
{
  mDevice = device;

  VkDevice vkDevice{ mDevice->getVkDevice() };

  mFences.resize(numFences);

  VkFenceCreateInfo fenceInfo{};

  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for(auto& fence : mFences)
  {
    VkResult result{ vkCreateFence(vkDevice, &fenceInfo, nullptr, &fence) };

    if(result not_eq VK_SUCCESS)
    {
      DEBUG_CALLBACK(error,
                     "Failed to create a fence.",
                     vkCreateFence);

      return failure;
    }
  }

  return success;
}

template<typename FC>
VkFence&
so::vk::Fences<FC>::operator[](const index_t idx)
{
  return mFences[static_cast<Fences<FC>::size_type>(idx)];
}
