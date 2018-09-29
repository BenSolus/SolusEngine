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
 *  @file      cxx/soMemory.hpp
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

#include <memory>

namespace so {

#if (__cplusplus > 201103L || defined(_MSC_VER)) && \
     !(defined(__GNUC__) && (__GNUC__ == 4) && (__GNUC_MINOR__ == 8))

using std::make_unique;

#else

namespace internal {

template <typename T>
struct UniqueIf
{
  using scalar = std::unique_ptr<T>;
};

template <typename T>
struct UniqueIf<T[]>
{
  using arrayUnknownBound = std::unique_ptr<T[]>;
};

template <typename T, size_type N>
struct UniqueIf<T[N]>
{
  using arrayKnownBound = void;
};

} // namespace internal

template <typename T, typename... Args>
typename internal::UniqueIf<T>::scalar
make_unique(Args&&... args)
{
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T>
typename internal::UniqueIf<T>::arrayUnknownBound
make_unique(size_type n)
{
  return std::unique_ptr<T>(new typename std::remove_extent<T>[n]());
}

template <typename T, typename... Args>
typename internal::UniqueIf<T>::arrayKnownBound
make_unique(Args&&... args) = delete;

#endif

} // namespace so

