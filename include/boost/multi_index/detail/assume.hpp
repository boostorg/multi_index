/* Copyright 2003-2026 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#ifndef BOOST_MULTI_INDEX_DETAIL_ASSUME_HPP
#define BOOST_MULTI_INDEX_DETAIL_ASSUME_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <boost/assert.hpp>

#ifdef __has_builtin
#define BOOST_MULTI_INDEX_HAS_BUILTIN(x) __has_builtin(x)
#else
#define BOOST_MULTI_INDEX_HAS_BUILTIN(x) 0
#endif

#if !defined(NDEBUG)
#define BOOST_MULTI_INDEX_ASSUME(cond) BOOST_ASSERT(cond)
#elif BOOST_MULTI_INDEX_HAS_BUILTIN(__builtin_assume)
#define BOOST_MULTI_INDEX_ASSUME(cond) __builtin_assume(cond)
#elif defined(__GNUC__) || \
      BOOST_MULTI_INDEX_HAS_BUILTIN(__builtin_unreachable)
#define BOOST_MULTI_INDEX_ASSUME(cond)  \
  do{                                   \
    if(!(cond))__builtin_unreachable(); \
  }while(0)
#elif defined(_MSC_VER)
#define BOOST_MULTI_INDEX_ASSUME(cond) __assume(cond)
#else
#define BOOST_MULTI_INDEX_ASSUME(cond) \
  do{                                  \
    static_cast<void>(false&&(cond));  \
  }while(0)
#endif

#endif
