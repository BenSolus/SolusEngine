/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      soException.hpp
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
#include <iostream>
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

namespace so
{
  namespace utils
  {
    namespace err
    {
      template <typename T>
      class
      Exception : public T
      {
        static_assert(std::is_base_of<std::exception, T>::value,
                      "Template is not derived from std::exception!");
        public:
          Exception() : T() {}

          Exception(std::string const& explanatoryString,
                    std::string const& explanatoryPrefix)
            : T(explanatoryPrefix + ": " + explanatoryString) {}

          void
          print(size_t const indent = 0) const
          {
            std::cerr << std::string(indent, ' ') << this->what() << '\n';

            try
            {
              std::rethrow_if_nested(*this);
            }
            catch(Exception const& next)
            {
              next.print(indent + 1);
            }
          }
      };
    } // err
  } // utils
} // so