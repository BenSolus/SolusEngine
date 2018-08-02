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
 *  @file      vk/interfaces/soVkSurface.hpp
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

#include <glfw/soGLFWSurfaceInterface.hpp>

#include <soVkInstance.hpp>

namespace so {
namespace vk {

template<SurfaceBackend SB>
class
SurfaceInterface : public so::SurfaceInterface<SB>
{
  public:
    SurfaceInterface()
      : so::SurfaceInterface<SB>(),
        mSurface(VK_NULL_HANDLE),
        mInstance(Instance::SHARED_PTR_NULL_INSTANCE)
    {}

    SurfaceInterface(SurfaceInterface const& other) = delete;

    SurfaceInterface(SurfaceInterface&& other) noexcept = delete;  

    SurfaceInterface(SharedPtrInstance instance)
      : so::SurfaceInterface<SB>(),
        mSurface(VK_NULL_HANDLE),
        mInstance(instance)
    {}

    SurfaceInterface(std::string const& title)
      : so::SurfaceInterface<SB>(title),
        mSurface(VK_NULL_HANDLE),
        mInstance(Instance::SHARED_PTR_NULL_INSTANCE)
    {}

    ~SurfaceInterface() noexcept
    {
      VkInstance const instance(mInstance->getVkInstance());

      if((mSurface not_eq VK_NULL_HANDLE) and (instance not_eq VK_NULL_HANDLE))
        destroySurfaceKHR(instance, mSurface, nullptr);
    }

    SurfaceInterface<SB>& operator=(SurfaceInterface<SB> const& other)
      = delete;

    SurfaceInterface<SB>&
    operator=(SurfaceInterface<SB>&& other)
    {
      if(this is_eq &other)
        return *this;

      so::SurfaceInterface<SB>::operator=
        (static_cast<so::SurfaceInterface<SB>&&>(other));

      mSurface  = other.mSurface;
      mInstance = other.mInstance;

      other.mSurface  = VK_NULL_HANDLE;
      other.mInstance = Instance::SHARED_PTR_NULL_INSTANCE;

      return *this;
    }

    inline auto getSharedPtrInstance() { return mInstance; }

  protected:
    VkSurfaceKHR      mSurface;

    SharedPtrInstance mInstance;

}; // class SurfaceInterface

template<SurfaceBackend SB>
class
Surface : public so::vk::SurfaceInterface<SB>
{};

} // namespace vk
} // namespace so

