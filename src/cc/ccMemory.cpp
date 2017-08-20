/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <ccMemory.hpp>

#if defined(_MSC_VER) || defined(__MINGW32__)

#else
#include <cstdlib>
#endif

void*
cc::alignedAlloc(std::size_t size, std::size_t alignment)
{
  void* data(nullptr);

#if defined(_MSC_VER) || defined(__MINGW32__)
  data = _aligned_malloc(size, alignment);
#else
  int result(posix_memalign(&data, alignment, size));

  if(result not_eq 0)
    data = nullptr;
#endif

  return data;
}

void
cc::alignedFree(void* data)
{
#if	defined(_MSC_VER) || defined(__MINGW32__)
  _aligned_free(data);
#else
  free(data);
#endif
}