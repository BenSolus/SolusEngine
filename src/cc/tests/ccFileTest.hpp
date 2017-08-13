/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      ccFileTest.hpp
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

#include <ccConstantsTest.hpp>
#include <ccFile.hpp>

const std::string loremRef
  ("Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod "
   "tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim "
   "veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea "
   "commodo consequat. Duis aute irure dolor in reprehenderit in voluptate "
   "velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat "
   "cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id "
   "est laborum.\n");

TEST(ccFile, readFile)
{
  std::vector<char> lorem(cc::readFile(BIN_DIR + "/data/tests/lorem.txt"));

  ASSERT_EQ(std::string(lorem.data()).compare(loremRef), 0);
}

TEST(ccFile, readFileException)
{
  ASSERT_ANY_THROW(cc::readFile("non-existing.txt"));
}
