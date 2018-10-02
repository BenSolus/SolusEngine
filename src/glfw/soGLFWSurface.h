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
 *  @file      soGLFWSurface.h
 *  @author    Bennet Carstensen
 *  @date      2018
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

#include "soVulkanSurface.hpp"

#include "soReturnT.hpp"

#ifdef __cplusplus

extern "C"
{

#endif // __cplusplus

so::return_t
soVkGLFWSurfaceInitialize(void** surface);

so::return_t
soVkGLFWGetInstanceExtensions(void const*    surface,
                              char const***  extensions,
                              so::size_type* count);

void
soVkGLFWSurfaceTerminate(void* surface);

so::return_t
soVkGLFWSurfaceCreateWindow(void*                surface,
                            std::string   const& title,
                            so::size_type const  width,
                            so::size_type const  height);

so::return_t
soVkGLFWSurfaceCreateSurface(void* surface, VkInstance instance);

VkSurfaceKHR
soVkGLFWSurfaceGetVkSurfaceKHR(void const* surface);

bool
soVkGLFWSurfaceWindowIsClosed(void* surface);

void
soVkGLFWSurfacePollEvents();

void
soGLFWSurfaceGetWindowSize(void*          surface,
                           so::size_type* width,
                           so::size_type* height);

#ifdef __cplusplus

} // extern "C"

#endif // __cplusplus


