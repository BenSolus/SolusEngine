/*
 * Copyright (C) 2017 by Bennet Carstensen
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
 *  @file      soModule.hpp
 *  @author    Bennet Carstensen
 *  @date      2017
 *  @copyright Copyright (c) 2017 Bennet Carstensen
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

#include <soDynamicLibrary.hpp>

#include <soFileSystem.hpp>
#include <soJSON.hpp>

#include <map>

namespace so
{

enum class
EngineBackend
{
  Vulkan
};

std::string
to_string(EngineBackend backend);

class
VoidReturn
{
  public:
    static VoidReturn const&
    getVoidReturn();

    VoidReturn(VoidReturn const& other) = delete;

    VoidReturn(VoidReturn&& other) noexcept = delete;

    ~VoidReturn() noexcept = default;

    VoidReturn& operator=(VoidReturn const& other) = delete;

    VoidReturn& operator=(VoidReturn&& other) = delete;
 
  private:
    VoidReturn() = default;
     
};

class
Module
{
  using Symbols = std::vector<base::Symbol>;
  
  public:
    Module();

    Module(Path const& configFile, EngineBackend const backend);

    Module(Module const& other) = delete;

    Module(Module&& other) noexcept;

    ~Module() noexcept = default;

    Module& operator=(Module const& other) = delete;

    Module& operator=(Module&& other) noexcept = delete;

    template<typename... Args>
    void
    operator()(index const idx, VoidReturn const& voidReturn, Args... args)
    {
      (void) voidReturn;

      auto const pos(static_cast<Symbols::size_type>(idx));

      static_cast<so::Symbol<void, Args...>&>(mSymbols[pos])(args...);
    }

    template<typename Ret, typename... Args>
    void
    operator()(index const idx, Ret& ret, Args... args)
    {
      auto const pos(static_cast<Symbols::size_type>(idx));

      ret = static_cast<so::Symbol<Ret, Args...>&>(mSymbols[pos])(args...);
    }

    template<typename Ret, typename... Args>
    void
    operator()(index const idx, Ret& ret, Args... args) const
    {
      auto const pos(static_cast<Symbols::size_type>(idx));

      ret = static_cast<so::Symbol<Ret, Args...>&>(mSymbols[pos])(args...);
    }


    base::Symbol const&
    operator[](index const idx);

    base::Symbol const&
    getSymbol(index const idx);

    std::string const
    getName();

    std::string const
    getName() const;
    bool
    isAvailable();

  private:
    std::string    mName;

    DynamicLibrary mLibrary;
    mutable Symbols        mSymbols;

    bool           mIsAvailable;

    void 
    loadLibrary(JSON const& platformSpecifics);

    void
    loadSymbols(JSON const& implementation);
};

} // namespace so

