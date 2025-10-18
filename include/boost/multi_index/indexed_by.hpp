/* Copyright 2003-2025 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#ifndef BOOST_MULTI_INDEX_INDEXED_BY_HPP
#define BOOST_MULTI_INDEX_INDEXED_BY_HPP

#if defined(_MSC_VER)
#pragma once
#endif

/* An Mp11 list containing the index specifiers for instantiation
 * of a multi_index_container.
 */

namespace boost{

namespace multi_index{

template<typename T,typename... Ts>
struct indexed_by;

} /* namespace multi_index */

} /* namespace boost */

#endif
