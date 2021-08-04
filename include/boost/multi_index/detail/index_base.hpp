/* Copyright 2003-2021 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#ifndef BOOST_MULTI_INDEX_DETAIL_INDEX_BASE_HPP
#define BOOST_MULTI_INDEX_DETAIL_INDEX_BASE_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <boost/core/addressof.hpp>
#include <boost/core/no_exceptions_support.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/move/utility_core.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/multi_index/detail/access_specifier.hpp>
#include <boost/multi_index/detail/allocator_traits.hpp>
#include <boost/multi_index/detail/copy_map.hpp>
#include <boost/multi_index/detail/do_not_copy_elements_tag.hpp>
#include <boost/multi_index/detail/node_handle.hpp>
#include <boost/multi_index/detail/node_type.hpp>
#include <boost/multi_index/detail/vartempl_support.hpp>
#include <boost/multi_index_container_fwd.hpp>
#include <boost/tuple/tuple.hpp>
#include <utility>

#if !defined(BOOST_MULTI_INDEX_DISABLE_SERIALIZATION)
#include <boost/multi_index/detail/index_loader.hpp>
#include <boost/multi_index/detail/index_saver.hpp>
#endif

namespace boost{

namespace multi_index{

namespace detail{

/* The role of this class is threefold:
 *   - tops the linear hierarchy of indices.
 *   - terminates some cascading backbone function calls (insert_, etc.),
 *   - grants access to the backbone functions of the final
 *     multi_index_container class (for access restriction reasons, these
 *     cannot be called directly from the index classes.)
 */

struct lvalue_tag{};
struct rvalue_tag{};
struct emplaced_tag{};

#if 0 /* we'll use this later for iterator reassignment in merge */
template<typename MultiIndexContainer,int N=0>
struct iterator_sequence;

struct iterator_sequence_terminal
{
  iterator_sequence_terminal(void*){}
};

template<typename MultiIndexContainer,int N>
struct iterator_sequence_normal
{
  MultiIndexContainer* p;

  iterator_sequence_normal(MultiIndexContainer* p_):p(p_){}

  typename nth_index<MultiIndexContainer,N>::type::iterator
  get(){return p->template get<N>().end();}

  iterator_sequence<MultiIndexContainer,N+1>
  next(){return iterator_sequence<MultiIndexContainer,N+1>(p);}
};

template<typename MultiIndexContainer,int N>
struct iterator_sequence_base:
  mpl::if_c<
    N<mpl::size<typename MultiIndexContainer::index_type_list>::type::value,
    iterator_sequence_normal<MultiIndexContainer,N>,
    iterator_sequence_terminal
  >
{};

template<typename MultiIndexContainer,int N>
struct iterator_sequence:
  iterator_sequence_base<MultiIndexContainer,N>::type
{
  typedef typename iterator_sequence_base<
    MultiIndexContainer,N>::type               super;
 
  iterator_sequence(MultiIndexContainer* p):super(p){}
};
#endif

template<typename Value,typename IndexSpecifierList,typename Allocator>
class index_base
{
protected:
  typedef index_node_base<Value,Allocator>    index_node_type;
  typedef typename multi_index_node_type<
    Value,IndexSpecifierList,Allocator>::type final_node_type;
  typedef multi_index_container<
    Value,IndexSpecifierList,Allocator>       final_type;
  typedef tuples::null_type                   ctor_args_list;
  typedef typename rebind_alloc_for<
    Allocator,typename Allocator::value_type
  >::type                                     final_allocator_type;
  typedef node_handle<
    final_node_type,final_allocator_type>     final_node_handle_type;
  typedef mpl::vector0<>                      index_type_list;
  typedef mpl::vector0<>                      iterator_type_list;
  typedef mpl::vector0<>                      const_iterator_type_list;
  typedef copy_map<
    final_node_type,
    final_allocator_type>                     copy_map_type;

#if !defined(BOOST_MULTI_INDEX_DISABLE_SERIALIZATION)
  typedef index_saver<
    index_node_type,
    final_allocator_type>                     index_saver_type;
  typedef index_loader<
    index_node_type,
    final_node_type,
    final_allocator_type>                     index_loader_type;
#endif

private:
  typedef Value                               value_type;
  typedef allocator_traits<Allocator>         alloc_traits;
  typedef typename alloc_traits::size_type    size_type;

protected:
  explicit index_base(const ctor_args_list&,const Allocator&){}

  index_base(
    const index_base<Value,IndexSpecifierList,Allocator>&,
    do_not_copy_elements_tag)
  {}

  void copy_(
    const index_base<Value,IndexSpecifierList,Allocator>&,const copy_map_type&)
  {}

  final_node_type* insert_(const value_type& v,final_node_type*& x,lvalue_tag)
  {
    x=final().allocate_node();
    BOOST_TRY{
      final().construct_value(x,v);
    }
    BOOST_CATCH(...){
      final().deallocate_node(x);
      BOOST_RETHROW;
    }
    BOOST_CATCH_END
    return x;
  }

  final_node_type* insert_(const value_type& v,final_node_type*& x,rvalue_tag)
  {
    x=final().allocate_node();
    BOOST_TRY{
      final().construct_value(x,boost::move(const_cast<value_type&>(v)));
    }
    BOOST_CATCH(...){
      final().deallocate_node(x);
      BOOST_RETHROW;
    }
    BOOST_CATCH_END
    return x;
  }

  final_node_type* insert_(const value_type&,final_node_type*& x,emplaced_tag)
  {
    return x;
  }

  template<typename Index>
  final_node_type* insert_(
    const value_type& v,final_node_type*& x,Index* p)
  {
    p->final_extract_for_merge_(x);
    return x;
  }

  final_node_type* insert_(
    const value_type& v,index_node_type*,final_node_type*& x,lvalue_tag)
  {
    return insert_(v,x,lvalue_tag());
  }

  final_node_type* insert_(
    const value_type& v,index_node_type*,final_node_type*& x,rvalue_tag)
  {
    return insert_(v,x,rvalue_tag());
  }

  final_node_type* insert_(
    const value_type&,index_node_type*,final_node_type*& x,emplaced_tag)
  {
    return x;
  }

  template<typename BoolConstant>
  void extract_(index_node_type*,BoolConstant /* invalidate_iterators */){}

  void clear_(){}

  template<typename BoolConstant>
  void swap_(
    index_base<Value,IndexSpecifierList,Allocator>&,
    BoolConstant /* swap_allocators */)
  {}

  void swap_elements_(index_base<Value,IndexSpecifierList,Allocator>&){}

  bool replace_(const value_type& v,index_node_type* x,lvalue_tag)
  {
    x->value()=v;
    return true;
  }

  bool replace_(const value_type& v,index_node_type* x,rvalue_tag)
  {
    x->value()=boost::move(const_cast<value_type&>(v));
    return true;
  }

  bool modify_(index_node_type*){return true;}

  bool modify_rollback_(index_node_type*){return true;}

  bool check_rollback_(index_node_type*)const{return true;}

#if !defined(BOOST_MULTI_INDEX_DISABLE_SERIALIZATION)
  /* serialization */

  template<typename Archive>
  void save_(Archive&,const unsigned int,const index_saver_type&)const{}

  template<typename Archive>
  void load_(Archive&,const unsigned int,const index_loader_type&){}
#endif

#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)
  /* invariant stuff */

  bool invariant_()const{return true;}
#endif

  /* access to backbone memfuns of Final class */

  final_type&       final(){return *static_cast<final_type*>(this);}
  const final_type& final()const{return *static_cast<const final_type*>(this);}

  final_node_type* final_header()const{return final().header();}

  bool      final_empty_()const{return final().empty_();}
  size_type final_size_()const{return final().size_();}
  size_type final_max_size_()const{return final().max_size_();}

  std::pair<final_node_type*,bool> final_insert_(const value_type& x)
    {return final().insert_(x);}
  std::pair<final_node_type*,bool> final_insert_rv_(const value_type& x)
    {return final().insert_rv_(x);}
  template<typename T>
  std::pair<final_node_type*,bool> final_insert_ref_(const T& t)
    {return final().insert_ref_(t);}
  template<typename T>
  std::pair<final_node_type*,bool> final_insert_ref_(T& t)
    {return final().insert_ref_(t);}
  std::pair<final_node_type*,bool> final_insert_nh_(final_node_handle_type& nh)
    {return final().insert_nh_(nh);}

  template<typename Index>
  void final_merge_(final_node_type* x,Index& i){return final().merge_(x,i);}

  template<BOOST_MULTI_INDEX_TEMPLATE_PARAM_PACK>
  std::pair<final_node_type*,bool> final_emplace_(
    BOOST_MULTI_INDEX_FUNCTION_PARAM_PACK)
  {
    return final().emplace_(BOOST_MULTI_INDEX_FORWARD_PARAM_PACK);
  }

  std::pair<final_node_type*,bool> final_insert_(
    const value_type& x,final_node_type* position)
    {return final().insert_(x,position);}
  std::pair<final_node_type*,bool> final_insert_rv_(
    const value_type& x,final_node_type* position)
    {return final().insert_rv_(x,position);}
  template<typename T>
  std::pair<final_node_type*,bool> final_insert_ref_(
    const T& t,final_node_type* position)
    {return final().insert_ref_(t,position);}
  template<typename T>
  std::pair<final_node_type*,bool> final_insert_ref_(
    T& t,final_node_type* position)
    {return final().insert_ref_(t,position);}
  std::pair<final_node_type*,bool> final_insert_nh_(
    final_node_handle_type& nh,final_node_type* position)
    {return final().insert_nh_(nh,position);}

  template<BOOST_MULTI_INDEX_TEMPLATE_PARAM_PACK>
  std::pair<final_node_type*,bool> final_emplace_hint_(
    final_node_type* position,BOOST_MULTI_INDEX_FUNCTION_PARAM_PACK)
  {
    return final().emplace_hint_(
      position,BOOST_MULTI_INDEX_FORWARD_PARAM_PACK);
  }

  final_node_handle_type final_extract_(final_node_type* x)
  {
    return final().extract_(x);
  } 

  void final_erase_(final_node_type* x){final().erase_(x);}

  void final_delete_node_(final_node_type* x){final().delete_node_(x);}
  void final_delete_all_nodes_(){final().delete_all_nodes_();}
  void final_clear_(){final().clear_();}

  void final_swap_(final_type& x){final().swap_(x);}

  bool final_replace_(
    const value_type& k,final_node_type* x)
    {return final().replace_(k,x);}
  bool final_replace_rv_(
    const value_type& k,final_node_type* x)
    {return final().replace_rv_(k,x);}

  template<typename Modifier>
  bool final_modify_(Modifier& mod,final_node_type* x)
    {return final().modify_(mod,x);}

  template<typename Modifier,typename Rollback>
  bool final_modify_(Modifier& mod,Rollback& back,final_node_type* x)
    {return final().modify_(mod,back,x);}

#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)
  void final_check_invariant_()const{final().check_invariant_();}
#endif

BOOST_MULTI_INDEX_PRIVATE_IF_MEMBER_TEMPLATE_FRIENDS:
#if !defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS)
  /* final_extract_for_merge_ accessed from index_base of dst container */

  template <typename,typename,typename> friend class index_base;
#endif

  void final_extract_for_merge_(final_node_type* x)
  {
    final().extract_for_merge_(x);
  } 
};

} /* namespace multi_index::detail */

} /* namespace multi_index */

} /* namespace boost */

#endif
