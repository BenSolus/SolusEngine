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
 *  @file      vk/soVkSurface.hpp
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

#include "soVkInstance.hpp"

#include "soModule.hpp"
#include "soReturnT.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace so {
namespace vk {

class
SurfaceBackend
{
  using SurfaceHandle = void*;

  public:
    SurfaceBackend();

    explicit SurfaceBackend(Path const& config);

    SurfaceBackend(std::string const& title, SharedPtrInstance instance);

    SurfaceBackend(SurfaceBackend const& other) = delete;

    SurfaceBackend(SurfaceBackend&& other) noexcept;

    ~SurfaceBackend() noexcept;

    SurfaceBackend&
    operator=(SurfaceBackend const& other) = delete;

    SurfaceBackend&
    operator=(SurfaceBackend&& other) noexcept = default;

    void
    initialize();

    std::vector<char const*>
    getInstanceExtensions() const;

    return_t
    createWindow(std::string const& title,
                 size_type   const  width,
                 size_type   const  height);

    return_t
    createSurface(VkInstance instance);

    VkSurfaceKHR
    getVkSurfaceKHR();

    bool
    windowIsClosed();

    void
    pollEvents();

    std::string const&
    getName();

    bool
    isAvailable();

  private:
    Module        mModule;

    SurfaceHandle mHandle;
};

class
Surface
{
	using Backends = std::vector<SurfaceBackend>;
  
  public:
    Surface();

		Surface(std::string const& title, SharedPtrInstance instance);

    Surface(Surface const& other) = delete;

    Surface(Surface&& other) noexcept = delete;

    ~Surface() noexcept = default;

    Surface&
    operator=(Surface const& other) = delete;

    Surface&
    operator=(Surface&& other) noexcept = default;

    void
    initialize();

    std::vector<char const*>
    getInstanceExtensions();

    return_t
    createWindow(std::string const& title,
                 size_type   const  width  = 800,
                 size_type   const  height = 600);

    return_t
    createSurface();

		bool
		windowIsClosed();

		void
		pollEvents();

		VkSurfaceKHR
    getVkSurfaceKHR();

		GLFWwindow*
    getGLFWwindow();

    void
    setSharedPtrInstance(SharedPtrInstance instance);

    SharedPtrInstance
		getSharedPtrInstance();

	private:
    Backends          mBackends;

    index             mActiveBackend;

    SharedPtrInstance mInstance;
};

} // namespace vk
} // namespace so
