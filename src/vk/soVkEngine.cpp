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

so::Engine::Engine()
  : mDebugCallback(),
    mSurface(),
    mSwapChain()
{}

so::return_t
so::Engine::initialize(std::string const& applicationName,
                       uint32_t    const  applicationVersion)
{
  so::return_t result;

  mSurface.initialize();
 
  vk::SharedPtrInstance instance(std::make_shared<vk::Instance>());

  result = instance->initialize(applicationName,
                                applicationVersion,
                                mSurface.getInstanceExtensions());
 
  if(result is_eq failure)
  {
    return failure;
  }

  mDebugCallback = vk::DebugReportCallbackEXT(instance);

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

  vk::SharedPtrLogicalDevice device
    (std::make_shared<vk::LogicalDevice>
       (instance, mSurface));

  mSwapChain     = vk::SwapChain(device, mSurface);

  return success;
}

