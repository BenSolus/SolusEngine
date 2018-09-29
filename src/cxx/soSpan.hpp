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
 *  @file      cxx/soSpan.hpp
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

#if (__cplusplus > 201103L)

#include "gsl/soGSLSpan.hpp"

#else // __cplusplus > 201103L 

namespace so {

template<class T, std::ptrdiff_t Extent>
class Span;

} // namespace so

template<class T, std::ptrdiff_t Extent>
std::ostream&
operator<<(std::ostream& stream, so::Span<T, Extent> const& span);

namespace std {

template<std::ptrdiff_t Extent>
int
strcmp(char const* lhs, so::Span<char const, Extent>& rhs);

} // namespace std

namespace so {

constexpr std::ptrdiff_t dynamicExtent{ -1 };

template<class T, std::ptrdiff_t Extent = dynamicExtent>
class Span
{
  using element_type = T;
  using index_type   = std::ptrdiff_t;
  using pointer      = T*;

  public:

    Span() noexcept : mData(nullptr) {}

    explicit Span(element_type(&arr)[Extent]) noexcept
      : mData(std::addressof(arr[0]))
    {
    }

    friend std::ostream&
    operator<<<T, Extent>(std::ostream&          stream,
                          Span<T, Extent> const& span);

    pointer data() const noexcept { return mData; }

  protected:
    static constexpr std::ptrdiff_t extent{ Extent };
    
    pointer    mData{ nullptr };

}; // class Span

} // namespace so

template<std::ptrdiff_t Extent>
std::ostream&
operator<<(std::ostream& stream, so::Span<char const, Extent> const& span)
{
  stream << span.data();
  
  return stream;
}

template<std::ptrdiff_t Extent>
int
std::strcmp(char const* lhs, so::Span<char const, Extent>& rhs)
{
  return std::strcmp(lhs, rhs.data());
}

#endif // else __cplusplus > 201103L 

