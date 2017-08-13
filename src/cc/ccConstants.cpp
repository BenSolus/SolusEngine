/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <ccConstants.hpp>

#include <ccException.hpp>

#include <cerrno>
#include <stdexcept>

#ifdef __linux__
#include <unistd.h>
#endif

static std::string
getBinDir();

const std::string BIN_DIR(getBinDir());

std::string
getBinDir()
{
  // Trying to find the root directory of the binarys.
  std::string root;

#ifdef __linux__ // Using readlink on Linux.

  char exe[FILENAME_MAX] = {};

  // Find path to executable
  if(readlink("/proc/self/exe", exe, FILENAME_MAX) == -1)
    cc::throw_with_nested<std::runtime_error>("failed to get path of this "
                                              "executable!",
                                              PRETTY_FUNCTION_SIG);
  else
    exe[sizeof(exe) - 1] = '\0';

  root = std::string(exe);

  // Remove executable name and the bin folder which will result in the
  // root directory. (Kinda hardcoded, but easily changeable.)

  const std::size_t pos(root.find("/bin/" +
                        std::string(program_invocation_short_name)));

  root = root.erase(pos, root.length() - pos);
#else
  cc::throw_with_nested<std::runtime_error>("failed to get path of binary "
                                            "directory: unsupported OS!");
#endif

  return root;
}
