/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <soFileSystem.hpp>

#include <soDefinitions.hpp>
#include <soException.hpp>

#ifdef __linux__
#include <unistd.h>
#endif

#include <fstream>

std::vector<char>
so::utils::fs::readBinaryFile(const std::string& filename)
{
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if(not file.is_open())
    throw err::Exception<std::runtime_error>("failed to open file!",
                                             PRETTY_FUNCTION_SIG);

  std::streamsize const fileSize(file.tellg());

  std::vector<char> buffer(static_cast<std::vector<char>::size_type>(fileSize));

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();

  return buffer;
}

static std::string
getBinDir()
{
  // Trying to find the root directory of the binarys.
  std::string root;

#ifdef __linux__ // Using readlink on Linux.

  char exe[FILENAME_MAX] = {};

  // Find path to executable
  if(readlink("/proc/self/exe", exe, FILENAME_MAX) is_eq -1)
  {
    throw so::utils::err::Exception<std::runtime_error>("failed to get path of "
                                                        "this executable!",
                                                        PRETTY_FUNCTION_SIG);
  }
  else
    exe[sizeof(exe) - 1] = '\0';

  root = std::string(exe);

  // Remove executable name and the bin folder which will result in the
  // root directory. (Kinda hardcoded, but easily changeable.)

  const std::size_t pos(root.find("bin/" +
                                  std::string(program_invocation_short_name)));

  root = root.erase(pos, root.length() - pos);
#else
  throw err::Exception<std::runtime_error>("failed to get path of binary "
                                           "directory: unsupported OS!");
#endif

  return root;
}

std::string const BIN_DIR(getBinDir());