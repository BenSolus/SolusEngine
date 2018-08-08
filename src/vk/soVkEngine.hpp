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

#include <interfaces/soEngineInterface.hpp>

#include <soVkDebugReportCallbackEXT.hpp>
#include <soVkInstance.hpp>
#include <soVkLogicalDevice.hpp>
#include <soVkSurface.hpp>

namespace so
{

template<SurfaceBackend SB>
class
Engine<EngineBackend::Vulkan, SB>
{
  public:
    Engine() : mDebugCallback(), mSurface() { }

    Engine(std::string const& applicationName,
           uint32_t    const  applicationVersion)
      : Engine()
    {
      try
      {
        vk::SharedPtrInstance instance
          (std::make_shared<vk::Instance>
            (applicationName, applicationVersion));

        mDebugCallback = vk::DebugReportCallbackEXT(instance);
          
        mSurface       = vk::Surface<SB>(applicationName, instance);

        vk::SharedPtrLogicalDevice device
          (std::make_shared<vk::LogicalDevice>
            (instance, mSurface.getVkSurfaceKHR()));
      }
      catch(...)
      {
        THROW_NESTED_EXCEPTION("failed to create the Solus Engine.");
        //throw so::Exception<std::exception>("TEST", PRETTY_FUNCTION_SIG);
        //throw EXCEPTION("failed to create the Solus Engine!");
      }
    }

    ~Engine() noexcept { }

    void
    createSurface(std::string const& title = "soEngine (Vulkan backend)")
    {
      mSurface = std::move(vk::Surface<SB>(title,
                                           mSurface.getSharedPtrInstance()));
    }

    inline auto surfaceIsClosed() { return mSurface.isClosed(); } 

    inline void surfacePollEvents() { mSurface.pollEvents(); } 

  private:
    vk::DebugReportCallbackEXT mDebugCallback;
    vk::Surface<SB>            mSurface;
};

} // namespace so
