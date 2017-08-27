/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      soAlignedVector.hpp
 *  @author    Bennet Carstensen
 *  @date      2017
 *  @copyright Copyright (c) 2017 Bennet Carstensen
 *
 *             This Source Code Form is subject to the terms of the
 *             Mozilla Public License, v. 2.0. If a copy of the MPL was
 *             not distributed with this file, You can obtain one at
 *             http://mozilla.org/MPL/2.0/.
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