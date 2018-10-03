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

#include "soDefinitions.hpp"
#include "soFileSystem.hpp"

#if defined(_MSC_VER) || defined(__MINGW32__)

static_assert(false, "Windows implementation of this module not provided.");

#elif defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))

#include <unistd.h>

static_assert(_POSIX_VERSION >= 200112L, "ISO POSIX.1-2001 "
                                         "(_POSIX_VERSION >= 200112L) "
                                         "required.");

#include <dlfcn.h>

#else 

#error "No known implementation to dynamically loading libraries for this "
       "compiler."

#endif

#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace so {
namespace base {

class
Symbol
{
  public:
    Symbol();
    
    explicit Symbol(void* symbol);

    Symbol(Symbol const& other) = delete;
    
    Symbol(Symbol&& other) noexcept;
 
    virtual ~Symbol() noexcept = default;

    Symbol& operator=(Symbol const& other) = delete;

    Symbol&
    operator=(Symbol&& other) noexcept;

    bool
    isValid();

    inline void setAddress(void* address) { mSymbol = address; }

  protected:
    void* mSymbol;

}; // class Symbol

} // namespace base

template<typename Ret, typename... Args>
class
Symbol : public base::Symbol
{
  public:
    Symbol() : base::Symbol() {}

    Symbol(Symbol const& other) = delete;
   
    Symbol(Symbol&& other) noexcept
      : base::Symbol(static_cast<base::Symbol&&>(other))
    {}
 
    explicit Symbol(void* symbol) : base::Symbol(symbol) {}

    virtual ~Symbol() noexcept = default;

    Symbol& operator=(Symbol const& other) = delete;

    Symbol&
    operator=(Symbol&& other) noexcept
    {
      if(this is_eq &other)
      {
        return *this;
      }

      base::Symbol::operator=(static_cast<base::Symbol&&>(other));
    
      return *this;
    }

    Ret
    operator()(Args... args)
    {
      Ret(*f)(Args...)(nullptr);

      *reinterpret_cast<void**>(&f) = mSymbol;

      return f(args...);
    }

    Ret
    operator()(Args... args) const
    {
      Ret(*f)(Args...)(nullptr);

      *reinterpret_cast<void**>(&f) = mSymbol;

      return f(args...);
    }

}; // class Symbol

class
DynamicLibrary
{
  using nameSymbolPair = std::pair<char const*, base::Symbol&>;

  public:
    DynamicLibrary();

    explicit DynamicLibrary(Path& file);

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
      {
        loadSymbols(symbol, next...);
      }
#endif
    } 

    DynamicLibrary(char const*                  file,
                   std::vector<nameSymbolPair>& symbols);

    DynamicLibrary(DynamicLibrary const& other) = delete;

    DynamicLibrary(DynamicLibrary&& other) noexcept;

    virtual ~DynamicLibrary() noexcept;

    DynamicLibrary& operator=(DynamicLibrary const& other) = delete;

    DynamicLibrary&
    operator=(DynamicLibrary&& other) noexcept;

    base::Symbol
    loadSymbol(std::string const& symbol);

    inline bool isComplete() { return mIsComplete; }

    inline bool isComplete() const { return mIsComplete; }

  private:
    bool  mIsComplete;
    void* mHandle;
 
    void
    loadSymbol(nameSymbolPair& symbol);

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

    void
    deleteMembers();

    void loadSymbols() { mIsComplete = true; }
}; // class DynamicLibrary

} // namespace so

