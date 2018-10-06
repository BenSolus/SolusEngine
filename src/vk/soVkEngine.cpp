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

#include "soVkEngine.hpp"

#include "cxx/soDebugCallback.hpp"
#include "cxx/soDefinitions.hpp"

so::Engine::Engine()
  : mDebugCallback(),
    mSurface(), 
    mSwapChain(),
    mRenderPass(),
    mPipeline(),
    mFramebuffers()
{}

so::return_t
so::Engine::initialize(std::string const& applicationName,
                       uint32_t    const  applicationVersion)
{
  so::return_t result;

  if(mSurface.initialize() is_eq failure)
  {
    return failure;
  }
 
  vk::SharedPtrInstance instance(std::make_shared<vk::Instance>());

  std::vector<char const*> instanceExtensions;

  if(mSurface.getInstanceExtensions(instanceExtensions) is_eq failure)
  {
    return failure;
  }

  result = instance->initialize(applicationName,
                                applicationVersion,
                                instanceExtensions);
 
  if(result is_eq failure)
  {
    return failure;
  }

  if(mDebugCallback.initialize(instance) is_eq failure)
  {
    return failure;
  }
  
  mSurface.setSharedPtrInstance(instance);

  if(mSurface.createWindow(applicationName) is_eq failure)
  {
    return failure;
  }
  
  if(mSurface.createSurface() is_eq failure)
  {
    return failure;
  }
        //mSurface.initialize();
        //mSurface       = vk::Surface(applicationName, instance);

  vk::SharedPtrLogicalDevice device(std::make_shared<vk::LogicalDevice>());

  result = device->initialize(instance, mSurface);

  if(result is_eq failure)
  {
    return failure;
  }

  if(mSwapChain.initialize(device, mSurface) is_eq failure)
  {
    return failure;
  }

  if(mRenderPass.initialize(device, mSwapChain) is_eq failure)
  {
    std::string message{ ": Failed to create a render pass." };
 
    PREPEND_FUNCTION_SIG_TO_STRING(message);

    executeDebugCallback(error, message);
  
    return failure;
  }

  if(mPipeline.initialize(device, mSwapChain, mRenderPass) is_eq failure)
  {
    std::string message{ ": Failed to create a graphics pipeline." };

    PREPEND_FUNCTION_SIG_TO_STRING(message);

    executeDebugCallback(error, message);

    return failure;
  }

  if(mFramebuffers.initialize(device, mSwapChain, mRenderPass) is_eq failure)
  {
    std::string message{ ": Failed to create framebuffers." };

    PREPEND_FUNCTION_SIG_TO_STRING(message);

    return failure;
  }

  return success;
}

