/* Copyright 2003-2025 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#ifndef BOOST_MULTI_INDEX_DETAIL_IS_INDEX_LIST_HPP
#define BOOST_MULTI_INDEX_DETAIL_IS_INDEX_LIST_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <type_traits>

namespace boost{

namespace multi_index{

namespace detail{

template<typename T>
struct is_index_list:std::false_type{};

/* an index list is a non-empty Mp11 list */
template<template<typename...> class L,typename T,typename... Ts>
struct is_index_list<L<T,Ts...>>:std::true_type{};

} /* namespace multi_index::detail */

} /* namespace multi_index */

} /* namespace boost */

#endif
