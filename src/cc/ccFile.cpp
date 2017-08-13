/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <ccFile.hpp>

#include <ccException.hpp>

#include <fstream>
#include <stdexcept>

std::vector<char>
cc::readFile(const std::string& filename)
{
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if(!file.is_open())
    cc::throw_with_nested<std::runtime_error>("failed to open file!",
                                              std::string(PRETTY_FUNCTION_SIG));

  const std::streamsize fileSize(file.tellg());

  std::vector<char> buffer(static_cast<std::vector<char>::size_type>(fileSize));

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();

  return buffer;
}
