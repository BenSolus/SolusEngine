/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      ccException.hpp
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

#include <exception>
#include <string>

#if defined(__GNUC__) || defined(__clang__)
#define PRETTY_FUNCTION_SIG __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define PRETTY_FUNCTION_SIG __FUNCSIG__
#elif defined(__func__)
#define PRETTY_FUNCTION_SIG __func__
#else
#define PRETTY_FUNCTION_SIG __FUNCTION__
#endif

namespace cc
{
  void
  print_exception(const std::exception&        exc,
                  const std::string::size_type level = 0);

  template<typename T>
  void
  throw_with_nested
    (const std::string& what, const std::string& prefix)
  {
    static_assert(std::is_base_of<std::exception, T>::value,
                  "Template is not derived from std::exception!");

    std::throw_with_nested(T(prefix + ": " + what));
  }
} // namespace fcm
