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

#include "soDefinitions.hpp"

#if (__cplusplus > 201103L)

#include "gsl/soGSLSpan.hpp"

#else

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

namespace internal {

template<std::ptrdiff_t Ext>
class
ExtentType
{
  public:
    using index_type = std::ptrdiff_t;

    static_assert(Ext >= 0, "A fixed-size span must be >= 0 in size.");

    constexpr ExtentType() noexcept = default;

    template<index_type Other>
    explicit constexpr ExtentType(ExtentType<Other> other)
    {
      static_assert((Other is_eq Ext) or (Other is_eq dynamicExtent),
                    "Mismatch between fixed-size extent and size of "
                    "initializing data.");

      static_assert(other.size() is_eq Ext, "Error");
    }

    explicit constexpr ExtentType(index_type size)
    {
      static_assert(size is_eq Ext, "Error");
    }

    constexpr index_type size() const noexcept { return Ext; }

}; // class ExtentType

template<>
class
ExtentType<dynamicExtent>
{
  public:
    using index_type = std::ptrdiff_t;
    
    template<index_type Other>
    explicit constexpr ExtentType(ExtentType<Other> other)
      : mSize(other.size())
    {
    }

    explicit constexpr ExtentType(index_type size) : mSize(size)
    {
      //static_assert(size >= 0, "Error");
    }

    constexpr index_type size() const noexcept { return mSize; }
  
  private:
    index_type mSize;
};

} // internal


template<class T, std::ptrdiff_t Extent = dynamicExtent>
class Span
{
  using element_type = T;
  using index_type   = std::ptrdiff_t;
  using pointer      = T*;

  struct NotNullptr
  {
    pointer ptr;
  };

  template<class ExtentType>
  class
  StorageType : public ExtentType
  {
    public:
      template<class OtherExtentType>
      constexpr StorageType(NotNullptr data, OtherExtentType ext)
        : ExtentType(ext), mData(data.ptr)
      {}

      constexpr pointer data() const noexcept { return mData; }

    private:
      pointer mData;
  };

  public:

    constexpr Span() noexcept : mStorage(nullptr, internal::ExtentType<0>()){}

    template <std::size_t N>
    explicit constexpr Span(element_type(&arr)[N]) noexcept
      : mStorage(NotNullptr{ std::addressof(arr[0]) },
                 internal::ExtentType<N> ())
    {
    }

    pointer data() const { return mStorage.data(); }

    friend std::ostream&
    operator<<<T, Extent>(std::ostream&          stream,
                          Span<T, Extent> const& span);

  protected:
    StorageType<internal::ExtentType<Extent>> mStorage;

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

