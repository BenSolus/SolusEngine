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

namespace so {
namespace vk {

class
Surface
{
  public:
    Surface();

		Surface(std::string const& title, SharedPtrInstance instance);

    Surface(Surface const& other) = delete;

    Surface(Surface&& other) noexcept = delete;

    ~Surface() noexcept;

    Surface&
    operator=(Surface const& other) = delete;

    Surface&
    operator=(Surface&& other) noexcept = delete;

    so::return_t
    initialize();

    so::return_t
    getInstanceExtensions(std::vector<char const*>& instanceExtensions);

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
    getVkSurfaceKHR() const;

    void
    getWindowSize(size_type& width, size_type& height) const;

    void
    setSharedPtrInstance(SharedPtrInstance const& instance);

    SharedPtrInstance
		getSharedPtrInstance();

	private:
    class Impl;

    std::unique_ptr<Impl> mPImpl;

    SharedPtrInstance     mInstance;
};

} // namespace vk
} // namespace so
