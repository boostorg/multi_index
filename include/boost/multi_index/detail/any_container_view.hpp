/* Copyright 2003-2021 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#ifndef BOOST_MULTI_INDEX_DETAIL_ANY_CONTAINER_VIEW_HPP
#define BOOST_MULTI_INDEX_DETAIL_ANY_CONTAINER_VIEW_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */

namespace boost{

namespace multi_index{

namespace detail{

/* TODO: WRITE */

template<typename ConstIterator>
struct any_container_view_vtable
{
  ConstIterator (*begin)(const void*);
  ConstIterator (*end)(const void*);
};

template<typename Container>
typename Container::const_iterator any_container_view_begin(const void* pc)
{
  return static_cast<const Container*>(pc)->begin();
}

template<typename Container>
typename Container::const_iterator any_container_view_end(const void* pc)
{
  return static_cast<const Container*>(pc)->end();
}

template<typename Container>
any_container_view_vtable<typename Container::const_iterator>*
any_container_view_vtable_for()
{
  static any_container_view_vtable<typename Container::const_iterator> vt=
  {
    &any_container_view_begin<Container>,
    &any_container_view_end<Container>
  };

  return &vt;
};

template<typename ConstIterator>
class any_container_view
{
public:
  template<typename Container>
  any_container_view(const Container& c):
    pc(&c),
    pvt(any_container_view_vtable_for<Container>())
    {}

  const void*   container()const{return pc;}
  ConstIterator begin()const{return pvt->begin(pc);}
  ConstIterator end()const{return pvt->end(pc);}

private:
  const void*                               pc;
  any_container_view_vtable<ConstIterator>* pvt;
};

} /* namespace multi_index::detail */

} /* namespace multi_index */

} /* namespace boost */

#endif
