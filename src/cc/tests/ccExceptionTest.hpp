/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      ccExceptionTest.hpp
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

#include <commonTest.hpp>

#include <ccException.hpp>

#include <iostream>

#if defined(__GNUC__) || defined(__clang__)
const std::string refWhat("void foo(): foo\n void bar(): bar\n");
#endif

static void foo();

static void bar();

TEST(ccException, throw_with_nested)
{
  // Expect to throw
  ASSERT_ANY_THROW(foo());
}

TEST(ccException, print_exception)
{
  // Redirect std::cerr.
  std::streambuf*    cerrBufBackup = std::cerr.rdbuf();
  std::ostringstream redirectedCerr;

  std::cerr.rdbuf(redirectedCerr.rdbuf());

  // cc::print_exception(e) goes to redirectedCerr.
  try { foo(); } catch(const std::exception& e) { cc::print_exception(e); }

  // Restore old std::cerr.
  std::cerr.rdbuf(cerrBufBackup);

  // Compare explanatory string of nested exception with reference string
  // refWhat
  ASSERT_EQ(redirectedCerr.str().compare(refWhat), 0);
}

void foo()
{
  try{ bar(); } catch(...)
  { cc::throw_with_nested<std::runtime_error>("foo", PRETTY_FUNCTION_SIG); }
}

void bar()
{ throw std::runtime_error(std::string(PRETTY_FUNCTION_SIG) + ": bar"); }
