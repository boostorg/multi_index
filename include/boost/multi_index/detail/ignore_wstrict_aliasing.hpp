/* Copyright 2003-2016 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#ifndef BOOST_MULTI_INDEX_DETAIL_IGNORE_WSTRICT_ALIASING_HPP
#define BOOST_MULTI_INDEX_DETAIL_IGNORE_WSTRICT_ALIASING_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/config.hpp>

#if defined(BOOST_GCC)&&(BOOST_GCC>=3*10000+3*100)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing" 
#endif

#endif
