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
 *  @file      soAlignedVector.hpp
 *  @author    Bennet Carstensen
 *  @date      2017
 *  @copyright Copyright (c) 2017 Bennet Carstensen
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

#include <utils/def/soDefinitions.hpp>
#include <utils/err/soException.hpp>

#include <cstring>
#include <limits>

namespace so
{
  namespace utils
  {
    namespace mem
    {
      template <typename T>
      class AlignedVector
      {
        public:
          using value_type      = T;
          using size_type       = std::size_t;
          using reference       = value_type&;
          using const_reference = const value_type&;

        public:
          explicit AlignedVector()
            : mAlignment(0), mNumComponents(0), mComponents(nullptr) {}

          explicit AlignedVector(size_type alignment, size_type count)
            : mAlignment(alignment), mNumComponents(count), mComponents(nullptr)
          {
            if(sizeof(T) > alignment)
              utils::err::Exception<std::runtime_error>
                ("Datatypes greater than the alignment currently not "
                 "supported!",
                 PRETTY_FUNCTION_SIG);

            void *data(nullptr);

            if(mNumComponents not_eq 0)
            {
#if defined(_MSC_VER) || defined(__MINGW32__)
              data = _aligned_malloc(mAlignment * mNumComponents, mNumComponents);
#else
              int result(posix_memalign(&data,
                                        mAlignment,
                                        mAlignment * mNumComponents));

              if (result not_eq 0)
                data = nullptr;
#endif
            }
            else
              data = nullptr;

            mComponents = static_cast<value_type*>(data);

            if((mNumComponents not_eq 0) and (mComponents is_eq nullptr))
              utils::err::Exception<std::runtime_error>("failed to allocate "
                                                        "aligned memory!",
                                                        PRETTY_FUNCTION_SIG);

            std::memset(mComponents, 0, mAlignment * mNumComponents);
          }

          AlignedVector(AlignedVector const& other) = delete;

          AlignedVector(AlignedVector&& other) = delete;

          ~AlignedVector() noexcept
          {
            destroyMembers();
          }

          AlignedVector& operator=(AlignedVector const& other) = delete;

          AlignedVector& operator=(AlignedVector&& other)
          {
            destroyMembers();

            mAlignment     = other.mAlignment;
            mNumComponents = other.mNumComponents;
            mComponents    = other.mComponents;

            other.mAlignment     = 0;
            other.mNumComponents = 0;
            other.mComponents    = nullptr;

            return *this;
          }

          /************************** Element access **************************/

          reference operator[](size_type pos)
          {
            return *reinterpret_cast<value_type*>
            /*     ^ Get actual value at address. */
              (reinterpret_cast<uint8_t*>(mComponents) + (pos * mAlignment));
            /* ^ Cast to move by bytes.                  ^ Move to next aligned
             *                                             position. */
          }

          const_reference operator[](size_type pos) const
          {
            return *reinterpret_cast<value_type*>
            /*     ^ Get actual value at address. */
              (reinterpret_cast<uint8_t*>(mComponents) + (pos * mAlignment));
            /* ^ Cast to move by bytes.                  ^ Move to next aligned
             *                                             position. */
          }

          T* data() noexcept { return mComponents; }

          const T* data() const noexcept { return mComponents; }

          /***************************** Capacity *****************************/

          inline auto empty() const noexcept { return mNumComponents is_eq 0; }

          inline auto size() const noexcept { return mNumComponents; }

          inline auto max_size() const noexcept
          { return std::numeric_limits<size_type>::max(); }

          inline auto alignment() const noexcept { return mAlignment; }

        private:
          size_type   mAlignment;

          size_type   mNumComponents;

          value_type* mComponents;

          void
          destroyMembers()
          {
            if (mComponents != nullptr)
#if  defined(_MSC_VER) || defined(__MINGW32__)
              _aligned_free(mComponents);
#else
              free(mComponents);
#endif
          }
      };
    } // mem
  } // utils
} // so