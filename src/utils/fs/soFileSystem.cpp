/*
 * Copyright (C) 2017 by Bennet Carstensen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
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
  /* Trying to find the root directory of the binarys. */
  std::string root;

#ifdef __linux__ // Using readlink on Linux.

  char exe[FILENAME_MAX] = {};

  // Find path to executable
  if(readlink("/proc/self/exe", exe, FILENAME_MAX) is_eq -1)
  {
    throw so::utils::err::Exception<std::runtime_error>(
      "failed to get path of "
      "this executable!",
      PRETTY_FUNCTION_SIG);
  }
  else
    exe[sizeof(exe) - 1] = '\0';

  root = std::string(exe);

  /* Remove executable name and the bin folder which will result in the
   * root directory. (Kinda hardcoded, but easily changeable.) */

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

