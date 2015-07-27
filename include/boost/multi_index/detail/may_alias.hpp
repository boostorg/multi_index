/* Copyright 2003-2015 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#ifndef BOOST_MULTI_INDEX_DETAIL_MAY_ALIAS_HPP
#define BOOST_MULTI_INDEX_DETAIL_MAY_ALIAS_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */

#if defined(BOOST_GCC)&&(__GNUC__>=4) ||\
    defined(BOOST_INTEL)&&(BOOST_INTEL_CXX_VERSION>=1300)
#define BOOST_MULTI_INDEX_ATTRIBUTE_MAY_ALIAS __attribute__((__may_alias__))
#else
#define BOOST_MULTI_INDEX_ATTRIBUTE_MAY_ALIAS
#endif

#endif
