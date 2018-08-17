/* Boost.MultiIndex test for terse key specification syntax.
 *
 * Copyright 2003-2018 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#include "test_key.hpp"

#if __cplusplus<201703L

void test_key()
{
  /* trivially passes as key is not available for pre-C++17 compilers */
}

#else

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <boost/detail/lightweight_test.hpp>
#include "pre_multi_index.hpp"
#include <boost/multi_index/key.hpp>
#include <functional>
#include <type_traits>

using namespace boost::multi_index;

namespace {

struct base
{
  int       x;
  const int cx;
  int       f(){return x;};
  int       cf()const{return x;};
};

int gf(const base& b){return b.x;}

struct derived:base
{
  int y;
};

int gh(derived& d){return d.y;}
int grh(std::reference_wrapper<derived>& d){return d.get().y;}

} /* namespace */

void test_key()
{
  BOOST_TEST((std::is_same_v<
    key<&base::x>,member<base,int,&base::x> 
  >));
  BOOST_TEST((std::is_same_v<
    key<&base::cx>,member<base,const int,&base::cx> 
  >));
  BOOST_TEST((std::is_same_v<
    key<&base::f>,mem_fun<base,int,&base::f> 
  >));
  BOOST_TEST((std::is_same_v<
    key<&base::cf>,const_mem_fun<base,int,&base::cf>
  >));
  BOOST_TEST((std::is_same_v<
    key<gf>,global_fun<const base&,int,gf> 
  >));
  BOOST_TEST((std::is_same_v<
    key<&base::x,&base::cx,&base::f,&base::cf,gf>,
    composite_key<
      base,
      member<base,int,&base::x>,
      member<base,const int,&base::cx>,
      mem_fun<base,int,&base::f>,
      const_mem_fun<base,int,&base::cf>,
      global_fun<const base&,int,gf>
    >
  >));
  BOOST_TEST((std::is_same_v<
    key<&base::x,&derived::y>,
    composite_key<
      derived,
      member<base,int,&base::x>,
      member<derived,int,&derived::y>
    >
  >));
  BOOST_TEST((std::is_same_v<
    key<gf,gh>,
    composite_key<
      derived,
      global_fun<const base&,int,gf>,
      global_fun<derived&,int,gh>
    >
  >));
  BOOST_TEST((std::is_same_v<
    key<gf,gh,grh>,
    composite_key<
      std::reference_wrapper<derived>,
      global_fun<const base&,int,gf>,
      global_fun<derived&,int,gh>,
      global_fun<std::reference_wrapper<derived>&,int,grh>
    >
  >));
}
#endif
