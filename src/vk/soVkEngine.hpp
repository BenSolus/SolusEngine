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
 *  @file      soEngine.hpp
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

#include "soVkCommandBuffers.hpp"
#include "soVkDebugReportCallbackEXT.hpp"
#include "soVkFences.hpp"
#include "soVkFramebuffers.hpp"
#include "soVkInstance.hpp"
#include "soVkLogicalDevice.hpp"
#include "soVkPipeline.hpp"
#include "soVkSemaphores.hpp"
#include "soVkSurface.hpp"

#include "cxx/soDefinitions.hpp"

namespace so {

class
Engine
{
  public:
    Engine();

    ~Engine() noexcept;

    so::return_t
    initialize(std::string const& applicationName,
               uint32_t    const  applicationVersion,
               size_type   const  maxFramesInFlight = 2);

    inline bool windowIsClosed() { return mSurface.windowIsClosed(); } 

    inline void surfacePollEvents() { mSurface.pollEvents(); } 

    inline VkDevice getVkDevice()
    { return mSwapChain.getDevice()->getVkDevice(); }
    
    return_t
    drawFrame();

  private:
    vk::DebugReportCallbackEXT mDebugCallback;
    vk::Surface                mSurface;
    vk::SwapChain              mSwapChain;
		vk::RenderPass             mRenderPass;
	  vk::Pipeline               mPipeline;
    vk::Framebuffers           mFramebuffers;
    vk::CommandBuffers         mCommandBuffers;
    vk::Semaphores             mImageAvailableSemaphores;
    vk::Semaphores             mRenderFinishedSemaphores;
    vk::Fences<>               mInFlightFences;

    index_t                    mCurrentFrame;

    bool                       mFramebuffersResized;

    return_t
    recreateSwapChain();

};

} // namespace so
