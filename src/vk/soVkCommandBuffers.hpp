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
 *  @file      soVkCommandBuffer.hpp
 *  @author    Bennet Carstensen
 *  @date      2017
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

#include "soVkCommandPool.hpp"
#include "soVkFramebuffers.hpp"
#include "soVkPipeline.hpp"

namespace so {
namespace vk {
   
class
CommandBuffers
{
  public:
    CommandBuffers();

    CommandBuffers(CommandBuffers const& other) = delete;

    CommandBuffers(CommandBuffers&& other) = delete;

    ~CommandBuffers() noexcept;

    CommandBuffers&
    operator=(CommandBuffers const& other) = delete;

    CommandBuffers&
    operator=(CommandBuffers&& other) noexcept;

    return_t
    initialize(SharedPtrLogicalDevice const& device,
               SharedPtrCommandPool   const& commandPool,
               Framebuffers           const& framebuffers,
               RenderPass             const& renderPass,
               SwapChain              const& swapChain,
               Pipeline               const& pipeline);

    return_t
    reset(Framebuffers const& framebuffers,
          RenderPass   const& renderPass,
          SwapChain    const& swapChain,
          Pipeline     const& pipeline);

    inline auto& getVkCommandBuffersRef() { return mCommandBuffers; }

  private:
    std::vector<VkCommandBuffer> mCommandBuffers;

    SharedPtrLogicalDevice       mDevice;
    SharedPtrCommandPool         mCommandPool;

    return_t
    initializeMembers(Framebuffers const& framebuffers,
                      RenderPass   const& renderPass,
                      SwapChain    const& swapChain,
                      Pipeline     const& pipeline);

    void
    destroyMembers();

}; // class CommandBuffers

} // namespace vk
} // namespace so
