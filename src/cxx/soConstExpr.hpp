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
 *  @file      cxx/soConstExpr.hpp
 *  @author    Bennet Carstensen
 *  @date      2017
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

#if __cplusplus < 201703L

namespace so {

template<typename IF, typename THEN>
constexpr typename std::enable_if<IF::value>::type
constExprIf(THEN&& trueStatement);

template<typename IF, typename THEN>
constexpr typename std::enable_if<not IF::value>::type
constExprIf(THEN&& trueStatement);

template<typename IF, typename THEN, typename ELSE>
constexpr typename std::enable_if<IF::value>::type
constExprIf(THEN&& trueStatement, ELSE&& falseStatement);

template<typename IF, typename THEN, typename ELSE>
constexpr typename std::enable_if<not IF::value>::type
constExprIf(THEN&& trueStatement, ELSE&& falseStatement);

} // namespace so

template<typename IF, typename THEN>
constexpr typename std::enable_if<IF::value>::type
so::constExprIf(THEN&& trueStatement)
{
  trueStatement();
}

template<typename IF, typename THEN>
constexpr typename std::enable_if<not IF::value>::type
so::constExprIf(THEN&& trueStatement)
{
  (void) trueStatement();
}

template<typename IF, typename THEN, typename ELSE>
constexpr typename std::enable_if<IF::value>::type
so::constExprIf(THEN&& trueStatement, ELSE&& falseStatement)
{
  (void) falseStatement;

  trueStatement();
}

template<typename IF, typename THEN, typename ELSE>
constexpr typename std::enable_if<not IF::value>::type
constExprIf(THEN&& trueStatement, ELSE&& falseStatement)
{
  (void) trueStatement;

  falseStatement();
}

#else // __cplusplus < 201703L
 
namespace so {

template<typename IF, typename THEN>
constexpr void
constExprIf(THEN&& trueStatement);

template<typename IF, typename THEN, typename ELSE>
constexpr void
constExprIf(THEN&& trueStatement, ELSE&& falseStatement);

} // namespace so

template<typename IF, typename THEN>
constexpr void
so::constExprIf(THEN&& trueStatement)
{
  if constexpr (IF::value)
  {
    trueStatement();
  }
}

template<typename IF, typename THEN, typename ELSE>
constexpr void
so::constExprIf(THEN&& trueStatement, ELSE&& falseStatement)
{
  if constexpr (IF::value)
  {
    trueStatement();
  }
  else
  {
    falseStatement();
  }
}

#endif // else __cplusplus < 201703L
 

