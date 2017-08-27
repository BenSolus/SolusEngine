/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      soAlignedAllocator.hpp
 *  @author    Bennet Carstensen
 *  @date      2017
 *  @copyright Copyright (c) 2017 Bennet Carstensen
 *
 *             This Source Code Form is subject to the terms of the
 *             Mozilla Public License, v. 2.0. If a copy of the MPL was
 *             not distributed with this file, You can obtain one at
 *             http://mozilla.org/MPL/2.0/.
 */

#include <limits>
#include <memory>

#pragma once

namespace so
{
  namespace utils
  {
    namespace mem
    {
      template<typename T>
      struct pointer_traits
      {
        using reference       = T&;
        using const_reference = const T&;
      };

// Avoid declaring a reference to void with an empty specialization
      template<>
      struct pointer_traits<void> { };

      template<typename T = void>
      class AlignedAllocator : public pointer_traits<T>
      {
        public:
          using value_type         = T;
          using size_type          = std::size_t;
          using pointer            = T*;
          using const_pointer      = const T*;
          using const_void_pointer = const void*;
          using difference_type    =
            typename std::pointer_traits<pointer>::difference_type;

          AlignedAllocator(std::size_t alignment) noexcept
            : mAlignment(alignment) {}

          ~AlignedAllocator() noexcept {}

          template<typename U>
          AlignedAllocator(const AlignedAllocator<U>& other) noexcept
            : mAlignment(other.mAlignment) {}

          inline pointer
          allocate(size_t n, const_void_pointer = 0)
          {
            void* data(nullptr);

#if defined(_MSC_VER) || defined(__MINGW32__)
            data = _aligned_malloc(n, mAlignment);
#else
            int result(posix_memalign(&data, mAlignment, n));

            if(result not_eq 0)
              data = nullptr;
#endif

            return data;
          }

          void
          deallocate(pointer p, size_type n)
          {
#if	defined(_MSC_VER) || defined(__MINGW32__)
            _aligned_free(p);
#else
            free(p);
#endif
          }

          inline size_type max_size() const
          { return std::numeric_limits<size_type>::max() / sizeof(T); }

          template<typename U>
          struct rebind { typedef AlignedAllocator<U> other; };

        private:
          std::size_t mAlignment;
      };

      template<typename T, typename U>
      constexpr bool operator==(const AlignedAllocator<T>&,
                                const AlignedAllocator<U>&) noexcept;

      template<typename T, typename U>
      constexpr bool operator!=(const AlignedAllocator<T>&,
                                const AlignedAllocator<U>&) noexcept;
    } // mem
  } // utils
} // so
