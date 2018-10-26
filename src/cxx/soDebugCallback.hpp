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
 *  @file      soDebugCallback.hpp
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

#include "soDefinitions.hpp"
#include "soMacroDispatcher.hpp"
#include "soPrettyFunctionSig.hpp"
#include "soReturnT.hpp"

namespace so {

enum class
DebugCode
{
  info,
  infoItem,
  verbose,
  error
};

typedef void (*DebugCallback) (DebugCode   const,
                               std::string const&,
                               std::string const&,
                               index_t     const,
                               std::string const&);

void
setDebugCallback(DebugCallback callback);

void
executeDebugCallback(DebugCode   const  code,
                     std::string const& message,
                     std::string const& funcSig,
                     index_t     const  line,
                     std::string const& file);

} // namespace so

#define DEBUG_CALLBACK(...) MACRO_DISPATCHER(DEBUG_CALLBACK, __VA_ARGS__)

#define DEBUG_CALLBACK2(code, message)          \
  so::executeDebugCallback(code,                \
                           message,             \
                           PRETTY_FUNCTION_SIG, \
                           __LINE__,            \
                           __FILE__)

#define DEBUG_CALLBACK3(code, message, function)                        \
  so::executeDebugCallback(code,                                        \
                           message,                                     \
                           getPrettyFunctionSig<decltype(&function)>(), \
                           __LINE__,                                    \
                           __FILE__)

#define DEBUG_CALLBACK4(code, message, trgClass, function)  \
  so::executeDebugCallback                               \
    (code,                                               \
     message,                                            \
     getPrettyFunctionSig<decltype(trgClass::*function)>(), \
     __LINE__,                                           \
     __FILE__)

constexpr so::DebugCode info     = so::DebugCode::info;

constexpr so::DebugCode infoItem = so::DebugCode::infoItem;

constexpr so::DebugCode verbose  = so::DebugCode::verbose;

constexpr so::DebugCode error    = so::DebugCode::error;

