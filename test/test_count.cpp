/* Boost.MultiIndex count test.
 *
 * Copyright 2022 Damian Sawicki.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#include "test_count.hpp"

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <vector>
#include <set>
#include "pre_multi_index.hpp"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/ranked_index.hpp>
#include <boost/detail/lightweight_test.hpp>

using namespace boost::multi_index;

struct employee
{
  int   id;
  int   age;

  employee(int id_,int age_):id(id_),age(age_){}
};

struct id{};
struct age_o{};
struct age_r{};

typedef multi_index_container<
  employee,
  indexed_by<
    ordered_unique<
      tag<id>,    BOOST_MULTI_INDEX_MEMBER(employee,int,id)>,
    ordered_non_unique<
      tag<age_o>, BOOST_MULTI_INDEX_MEMBER(employee,int,age)>,
    ranked_non_unique<
      tag<age_r>, BOOST_MULTI_INDEX_MEMBER(employee,int,age)> >
> employee_set;

void test_count_hardcoded()
{
  employee_set es;
  int last_id=0;
  for(int i=1;i<=10;++i){
    for(int j=0;j<2*i;++j){
      es.insert(employee(last_id++,40+i));
    }
  }
  for(uint i=0;i<=10;++i){
    BOOST_TEST(es.get<age_o>().count(40+i)==2*i);
    BOOST_TEST(es.get<age_r>().count(40+i)==2*i);
  }
  es.insert(employee(last_id++,60));
  for(int i=0;i<40;++i){
    BOOST_TEST(es.get<age_o>().count(i)==0);
    BOOST_TEST(es.get<age_r>().count(i)==0);
  }
  for(int i=51;i<60;++i){
    BOOST_TEST(es.get<age_o>().count(i)==0);
    BOOST_TEST(es.get<age_r>().count(i)==0);
  }
  BOOST_TEST(es.get<age_o>().count(60)==1);
  BOOST_TEST(es.get<age_r>().count(60)==1);
  for(int i=61;i<100;++i){
    BOOST_TEST(es.get<age_o>().count(i)==0);
    BOOST_TEST(es.get<age_r>().count(i)==0);
  }
}

const int decade=10;
struct smaller_decade
{
  bool operator()(int x,int y)const{return x/decade<y/decade;}
};

void test_count_random()
{
  const int max_age=100; // Should be a multiple of decade.
  int numbers_of_inserts[]={10,100,1000};
  // Relatively frequent vs unlikely failed insert:
  int id_ranges[]={1000,1000000000};
  const int random_loops=10;
  srand(time(NULL));
  const int max_x=sizeof(id_ranges)/sizeof(id_ranges[0]);
  for(int x=0;x<max_x;++x){
    int id_range=id_ranges[x];
    const int max_y=sizeof(numbers_of_inserts)/sizeof(numbers_of_inserts[0]);
    for(int y=0;y<max_y;++y){
      int number_of_inserts=numbers_of_inserts[y];
      for(uint i=0;i<random_loops;++i){
        employee_set random_set;
        std::set<int> used_ids;
        uint counts[max_age]={};
        for(int j=0;j<number_of_inserts;++j){
          int random_id=rand()%id_range;
          int random_age=rand()%max_age;
          std::pair<employee_set::iterator,bool> result=
            random_set.insert(employee(random_id,random_age));
          BOOST_TEST(result.second!=used_ids.count(random_id));
          if(result.second){
            used_ids.insert(random_id);
            ++counts[random_age];
          }
        }
        for(int k=0;k<max_age;++k){
          BOOST_TEST(counts[k]==random_set.get<age_r>().count(k));
          BOOST_TEST(counts[k]==random_set.get<age_o>().count(k));
          uint group=0;
          int decade_start=k/decade*decade;
          for(int j=decade_start;j<decade_start+decade;++j)
            group+=counts[j];
          BOOST_TEST(group==random_set.get<age_r>().count(k,smaller_decade()));
          BOOST_TEST(group==random_set.get<age_o>().count(k,smaller_decade()));
        }
      }
    }
  }
}

void test_count()
{
  test_count_hardcoded();
  test_count_random();
}
