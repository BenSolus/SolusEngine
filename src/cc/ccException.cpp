/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <ccException.hpp>

#include <iostream>

void
cc::print_exception(const std::exception&        except,
                    const std::string::size_type level)
{
  std::cerr << std::string(level, ' ') << except.what() << '\n';

  try
  {
    std::rethrow_if_nested(except);
  }
  catch(const std::exception& next)
  {
    print_exception(next, level+1);
  }
  catch(...)
  {}
}
