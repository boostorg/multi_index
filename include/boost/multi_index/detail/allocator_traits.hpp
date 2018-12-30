/* Copyright 2003-2018 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#ifndef BOOST_MULTI_INDEX_DETAIL_ALLOCATOR_TRAITS_HPP
#define BOOST_MULTI_INDEX_DETAIL_ALLOCATOR_TRAITS_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */

#if !defined(BOOST_NO_CXX11_ALLOCATOR)
#include <memory>
#else
#include <boost/core/pointer_traits.hpp>
#endif

namespace boost{

namespace multi_index{

namespace detail{

/* poor man's replacement of std::allocator_traits */

#if !defined(BOOST_NO_CXX11_ALLOCATOR)

template<typename Allocator>
struct allocator_traits:std::allocator_traits<Allocator>
{
  /* wrap std::allocator_traits alias templates for use in C++03 codebase */

  typedef std::allocator_traits<Allocator> super;

  template<typename U>
  struct rebind_alloc
  {
    typedef typename super::template rebind_alloc<U> type;
  };

  template<typename U>
  struct rebind_traits
  {
    typedef typename super::template rebind_traits<U> type;
  };
};

#else

/* not a full std::allocator_traits rewrite (not needed) */

template<typename Allocator>
struct allocator_traits
{

  typedef Allocator                           allocator_type;
  typedef typename Allocator::value_type      value_type;
  typedef typename Allocator::pointer         pointer;
  typedef typename Allocator::const_pointer   const_pointer;
  typedef typename pointer_traits<pointer>::
    template rebind_to<void>::type            void_pointer;
  typedef typename pointer_traits<pointer>::
    template rebind_to<const void>::type      const_void_pointer;
  typedef typename Allocator::difference_type difference_type;
  typedef typename Allocator::size_type       size_type;

  template<typename U>
  struct rebind_alloc
  {
    typedef typename Allocator::template rebind<U>::other type;
  };

  template<typename U>
  struct rebind_traits
  {
    typedef allocator_traits<typename rebind_alloc<U>::type> type;
  };

  static pointer   allocate(Allocator& a,size_type n){return a.allocate(n);}
  static pointer   allocate(Allocator& a,size_type n,const_void_pointer p)
                     {return a.allocate(n.static_cast<const_pointer>(p));}
  static void      deallocate(Allocator& a,pointer p,size_type n)
                     {a.deallocate(p,n);}
  static void      construct(Allocator& a,pointer p,const value_type& x)
                     {a.construct(p,x);}
  static size_type max_size(Allocator& a)BOOST_NOEXCEPT{return a.max_size();}
};

#endif

} /* namespace multi_index::detail */

} /* namespace multi_index */

} /* namespace boost */

#endif
