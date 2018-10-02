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

#include <soModule.hpp>

#include <fstream>
#include <iostream>

std::string
so::to_string(EngineBackend backend)
{
  switch(backend)
  {
    case EngineBackend::Vulkan:
      return "Vulkan";
  }

  return "Unkown Engine Backend";
}

so::VoidReturn const&
so::VoidReturn::getVoidReturn()
{
  static VoidReturn const voidReturn;

  return voidReturn;
}

so::Module::Module()
  : mName(""),
    mLibrary(),
    mSymbols(0),
    mIsAvailable(false)
{
}

so::Module::Module(Path const& configFile, EngineBackend const backend)
  : Module()
{
  std::ifstream configContent(configFile.string());

  JSON jsonContent;

  configContent >> jsonContent;

  mName = jsonContent["name"];
 
  loadLibrary(jsonContent["platform-specifics"]);

  if(not mLibrary.isComplete())
    return;

  mIsAvailable = true;

  JSON desiredImpl("");

  switch(backend)
  {
    case EngineBackend::Vulkan:
      for(auto const& implementation : jsonContent["implementations"])
        if(implementation["name"].get<std::string>() is_eq "Vulkan")
        {
          desiredImpl = implementation;
          break;
        }
      break;
  }
  
  if(desiredImpl is_eq JSON(""))
  {
    std::string warning("<WARNING> No implementation of '");

    warning += mName;
    warning += "' for engine backend '";
    warning += to_string(backend);
    warning += "'.\n";

    std::cerr << warning;

    return;
  }
 
  loadSymbols(desiredImpl);
}

so::Module::Module(Module&& other) noexcept
  : mName(std::move(other.mName)),
    mLibrary(std::move(other.mLibrary)),
    mSymbols(std::move(other.mSymbols)),
    mIsAvailable(other.mIsAvailable)
{
  other.mName        = "";
  other.mIsAvailable = false;
}

so::base::Symbol const&
so::Module::operator[](index const idx)
{
  return getSymbol(idx);
}

so::base::Symbol const&
so::Module::getSymbol(index const idx)
{
  auto const pos(static_cast<Symbols::size_type>(idx));

  return mSymbols[pos];
}

std::string const
so::Module::getName()
{
  return mName;
}

std::string const
so::Module::getName() const
{
  return mName;
}

bool
so::Module::isAvailable()
{
  return mIsAvailable;
}

void
so::Module::loadLibrary(JSON const& platformSpecifics)
{
  Path file(getBinaryDir());

  for(auto const& platform : platformSpecifics)
#if defined(_MSC_VER) || defined(__MINGW32__)

#  else
   if(platform["os"].get<std::string>() == "Linux")
#endif
     file /= platform["file"].get<std::string>();

  mLibrary = DynamicLibrary(file);

  if(not mLibrary.isComplete())
  {
    std::string warning("<WARNING> Couldn't load library file \n<WARNING>  ");

    warning += file.string();
    warning += ",\n<WARNING> needed by module '";
    warning += mName;
    warning += "'. Don't use this module.\n";

    std::cerr << warning; 
  }
}

void
so::Module::loadSymbols(JSON const& implementation)
{
  bool atLeastOneSymbolNotLoaded(false);

  for(auto const& symbol : implementation["symbols"])
  {
    auto symbolObject(mLibrary.loadSymbol(symbol["symbol"]));


    if(not symbolObject.isValid())
      atLeastOneSymbolNotLoaded = true;

    auto const& currentPos(mSymbols.begin() + symbol["index"]);
 
    mSymbols.insert(currentPos, mLibrary.loadSymbol(symbol["symbol"]));
  }

  if(atLeastOneSymbolNotLoaded)
  {
    std::string warning("<WARNING> At least one symbol couldn't be loaded. "
                        "Module '");

    warning += mName;
    warning += "' might not work the current engine backend.\n";

    std::cerr << warning;

    mIsAvailable = false;
  }
}

