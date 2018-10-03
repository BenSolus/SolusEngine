/*
 * Copyright (C) 2017-2018 by Bennet Carstensen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "soFileSystem.hpp"

#include "soConstExpr.hpp"
#include "soDefinitions.hpp"
#include "soOS.hpp"

#ifdef __linux__

#include <unistd.h>

#endif

#include <fstream>

so::return_t
so::readBinaryFile(const std::string& filename, std::vector<char>& content)
{
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if(not file.is_open())
  {
    return failure;
  }

  std::streamsize const fileSize(file.tellg());

  content.resize(static_cast<std::vector<char>::size_type>(fileSize)); 

  file.seekg(0);
  file.read(content.data(), fileSize);

  file.close();

  return success;
}

static std::string
getBinDir()
{
  /* Trying to find the root directory of the binarys. */
  std::string root;

  so::constExprIf<isUNIXBased<OS>> // if
  ([&]() // then
   {
     char exe[FILENAME_MAX] = {};

     // Find path to executable
     if(readlink("/proc/self/exe", exe, FILENAME_MAX) is_eq -1)
     {
       // TODO(BenSolus): Implementing some error handling.   
     }
     else
     {
       exe[sizeof(exe) - 1] = '\0';
     }

     root = std::string(exe);

     /* Remove executable name and the bin folder which will result in the
      * root directory. (Kinda hardcoded, but easily changeable.) */

     std::size_t const pos(root.find
                             ("bin/" +
                              std::string(program_invocation_short_name)));

     root = root.erase(pos, root.length() - pos);
   },
   [&](){} // else
  );

  return root;
}

so::Path const&
so::getBinaryDir()
{
  static so::Path const BinaryDir(getBinDir());

  return BinaryDir;
}

std::string const BIN_DIR(getBinDir());

