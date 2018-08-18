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

/**
 *  @file      cxx/soDynamicLibrary.hpp
 *  @author    Bennet Carstensen
 *  @date      2018
 *  @copyright Copyright (c) 2017-2018 Bennet Carstensen
 *
 *             Permission is hereby granted, free of charge, to any person
 *             obtaining a copy of this software and associated documentation
 *             files (the "Software"), to deal in the Software without
 *             restriction, including without limitation the rights to use,
 *             copy, modify, merge, publish, distribute, sublicense, and/or
 *             sell copies of the Software, and to permit persons to whom the
 *             Software is furnished to do so, subject to the following
 *             conditions:
 *
 *             The above copyright notice and this permission notice shall be
 *             included in all copies or substantial portions of the Software.
 *
 *             THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *             EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *             OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *             NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *             HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *             WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *             FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *             OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <soDefinitions.hpp>

#if defined(_MSC_VER) || defined(__MINGW32__)

static_assert(false, "Windows implementation of this module not provided.");

#elif defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))

#include <unistd.h>

static_assert(_POSIX_VERSION >= 200112L, "ISO POSIX.1-2001 "
                                         "(_POSIX_VERSION >= 200112L) "
                                         "required.");

#include <dlfcn.h>

#else 

#error "No known implementation to dynamically load libraries for this "
       "compiler."

#endif

#include <string>
#include <utility>

namespace so {

template<typename Ret, typename... Args>
class
Symbol
{
  public:
    Symbol() : mSymbol(nullptr) {}

    Symbol(Symbol const& other) : mSymbol(other.mSymbol) {}

    Symbol(Symbol& other) noexcept = delete;
 
    Symbol(void* symbol) : mSymbol(symbol) {}

    ~Symbol() { mSymbol = nullptr; }

    Symbol& operator=(Symbol const& other) = delete;

    Symbol&
    operator=(Symbol&& other) noexcept
    {
      if(this is_eq &other)
        return *this;

      mSymbol = other.mSymbol;

      other.mSymbol = nullptr;

      return *this;
    }

    Ret
    operator()(Args... args)
    {
      Ret(*f)(Args...)(nullptr);

      *reinterpret_cast<void**>(&f) = mSymbol;

      return f(args...);
    }
    
  private:
    void* mSymbol;

}; // class Symbol

class
DynamicLibrary
{
  public:
    template<typename Ret, typename... IN, typename... NEXT>
    DynamicLibrary(char const*                                 filename,
                   std::pair<char const*, Symbol<Ret, IN...>&> symbol,
                   NEXT...                                     next)
      : DynamicLibrary()
    {
#if defined(_MSC_VER) || defined(__MINGW32__)
      
#else
      mHandle = dlopen(filename, RTLD_NOW);

      (void) dlerror();

      if(mHandle not_eq nullptr)
        loadSymbols(symbol, next...);
#endif 
    } 

    DynamicLibrary(DynamicLibrary const& other) = delete;

    DynamicLibrary(DynamicLibrary&& other) noexcept = delete;

    ~DynamicLibrary() noexcept;

    DynamicLibrary& operator=(DynamicLibrary const& other) = delete;

    DynamicLibrary& operator=(DynamicLibrary&& other) noexcept = delete;

    inline auto isAvailable() const { return mIsAvailable; }

  private:
    bool  mIsAvailable;
    void* mHandle;

    DynamicLibrary();

    template<typename Ret, typename... IN, typename... NEXT>
    void
    loadSymbols(std::pair<char const*, Symbol<Ret, IN...>&>& symbol,
                NEXT...                                      next)
    {
#if defined(_MSC_VER) || defined(__MINGW32__)
#else
      void* symbolAddress(dlsym(mHandle, std::get<0>(symbol)));

      if(dlerror() is_eq nullptr)
      {
        std::get<1>(symbol) = Symbol<Ret, IN...>(symbolAddress);

        loadSymbols(next...);
      }
#endif
    }

    void loadSymbols() { mIsAvailable = true; }
}; // class DynamicLibrary

} // namespace so

