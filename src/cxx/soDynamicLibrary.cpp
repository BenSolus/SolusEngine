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

#include <soDynamicLibrary.hpp>

#include <soException.hpp>

#include "cxx/soConstExpr.hpp"
#include "cxx/soOS.hpp"

so::base::Symbol::Symbol() : mSymbol(nullptr) {}

so::base::Symbol::Symbol(Symbol&& other) noexcept
  : mSymbol(other.mSymbol)
{
  other.mSymbol = nullptr;
}

so::base::Symbol::Symbol(void* symbol) : mSymbol(symbol) {}

so::base::Symbol::~Symbol() noexcept { mSymbol = nullptr; }

so::base::Symbol&
so::base::Symbol::operator=(Symbol&& other) noexcept
{
  if(this is_eq &other)
    return *this;

  mSymbol = other.mSymbol;

  other.mSymbol = nullptr;

  return *this;
}

bool
so::base::Symbol::isValid()
{
  return mSymbol not_eq nullptr;
}

so::DynamicLibrary::DynamicLibrary() : mIsComplete(false), mHandle(nullptr) {}

so::DynamicLibrary::DynamicLibrary(Path& file)
  : DynamicLibrary()
{
  constExprIf<isUNIXBased<OS>> // if
  ([&]() // then
   {
     mHandle = dlopen(file.c_str(), RTLD_NOW);

     mIsComplete = mHandle not_eq nullptr;
    
     (void) dlerror();      
   },
   [&](){} // else
  );
}

so::DynamicLibrary::DynamicLibrary(char const*                  file,
                                   std::vector<nameSymbolPair>& symbols)
  : DynamicLibrary()
{
  constExprIf<isUNIXBased<OS>> // if
  ([&]() // then
   {
     mHandle = dlopen(file, RTLD_NOW);

     (void) dlerror();

     if(mHandle not_eq nullptr)
     { 
       mIsComplete = true;

       for(auto& symbol : symbols)
       {
         loadSymbol(symbol);
       }
     }
   },
   [&](){} // else
  );
}

so::DynamicLibrary::DynamicLibrary(DynamicLibrary&& other) noexcept
  : mIsComplete(other.mIsComplete), mHandle(other.mHandle)
{
  other.mIsComplete = false;
  other.mHandle     = nullptr;
}

so::DynamicLibrary::~DynamicLibrary() noexcept
{
  deleteMembers();
}

so::DynamicLibrary&
so::DynamicLibrary::operator=(DynamicLibrary&& other) noexcept
{
  if(this is_eq &other)
    return *this;

  deleteMembers();

  mIsComplete = other.mIsComplete;
  mHandle     = other.mHandle;

  other.mIsComplete = false;
  other.mHandle     = nullptr;

  return *this;
}

so::base::Symbol
so::DynamicLibrary::loadSymbol(std::string const& symbol)
{
#if defined(_MSC_VER) || defined(__MINGW32__)

#elif defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
  
  void* symbolAddress(dlsym(mHandle, symbol.c_str()));

  char const* lastError(dlerror());

  if(lastError is_eq nullptr)
    return base::Symbol(symbolAddress);
  else
  {
    std::string warning("<WARNING> Couldn't load symbol '");

    warning += symbol;
    warning += "': ";
    warning += lastError;
    warning += ".\n";

    std::cerr << warning;

    mIsComplete = false;

    return base::Symbol();
  }

#else

#endif  
}


void
so::DynamicLibrary::loadSymbol(nameSymbolPair& symbol)
{
#if defined(_MSC_VER) || defined(__MINGW32__)

#elif defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))

  void* symbolAddress(dlsym(mHandle, std::get<0>(symbol)));

  if(dlerror() is_eq nullptr)
    std::get<1>(symbol) = base::Symbol(symbolAddress);
  else
    mIsComplete = false;

#else

#endif
 
}

void
so::DynamicLibrary::deleteMembers()
{
#if defined(_MSC_VER) || defined(__MINGW32__)

#elif defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))

  if(mHandle not_eq nullptr)
    dlclose(mHandle);

#else 

#endif
}

