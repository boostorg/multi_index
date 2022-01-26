#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/ranked_index.hpp>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <time.h>

using boost::multi_index_container;
using namespace boost::multi_index;

/* an employee record holds its ID, name and age */

struct employee
{
  long long   id;
  std::string name;
  int         age;

  employee(long long id_,std::string name_,int age_):id(id_),name(name_),age(age_){}
};

const int decade=10;

struct smaller_decade
{
  bool operator()(int x,int y)const{return x/decade<y/decade;}
};

struct id{};
struct name{};
struct age{};

typedef multi_index_container<
  employee,
  indexed_by<
    ordered_unique<
      tag<id>,  BOOST_MULTI_INDEX_MEMBER(employee,long long,id)>,
    ordered_non_unique<
      tag<name>,BOOST_MULTI_INDEX_MEMBER(employee,std::string,name)>,
    ordered_non_unique<
      tag<age>, BOOST_MULTI_INDEX_MEMBER(employee,int,age)> >
> employee_set;

typedef multi_index_container<
  employee,
  indexed_by<
    ordered_unique<
      tag<id>,  BOOST_MULTI_INDEX_MEMBER(employee,long long,id)>,
    ordered_non_unique<
      tag<name>,BOOST_MULTI_INDEX_MEMBER(employee,std::string,name)>,
    ranked_non_unique<
      tag<age>, BOOST_MULTI_INDEX_MEMBER(employee,int,age)> >
> employee_ranked_set;

int main()
{
  std::cout<<"This program benchmarks two implementations, and the duration "
	     "of its execution may be considerable. Please feel free to stop "
	     "it at any point or play with the constants.\n";

  for(int variant=0;variant<2;++variant){
    employee_set es;
    employee_ranked_set ers;
    const int max_age=100;

    const int step=(variant==0)?1:decade;
    const int steps=std::ceil(((double)max_age)/step);

    std::cout<<"\nRunning the test with a comparison predicate differentiating "
             <<steps<<" groups of values of the index on which we call count "
             <<"(the actual number of distinct values is "<<max_age<<").\n";

    int last_id1=0,last_id2=0;

    for(int j=-1;j<11;++j){
      int size=es.size();
      int people=1000;
      switch(j){
	case -1:
	  people=100;
	  break;
	case 0:
	  people=900;
	  break;
	case 10:
	  people=90000;
	  break;
      }
      std::cout<<"Adding "<<people<<" new people to the multi_index";
      for(int i=0;i<people;++i){
	es.insert(employee(last_id1++,"Joe",i%max_age));
	ers.insert(employee(last_id2++,"Joe",i%max_age));
	if((10*i)%people==0)
	  std::cout<<"."<<std::flush;
      }
      std::cout<<std::endl;

      for(int iters=max_age;iters<=1000000;iters*=100){
	std::cout<<"Size "<<std::setw(6)<<es.size()<< ", "
		 <<std::setw(7)<<iters<<" calls of count()";
	clock_t start=std::clock();
	clock_t dur1=start-start;
	clock_t dur2=start-start;

	const int loops=std::sqrt(iters);
	for(int k=0;k<loops;++k){
	  if((10*k)%loops==0)
	    std::cout<<"."<<std::flush;

	  clock_t start1=std::clock();
	  for(int i=0;i<iters/loops;++i){
	    int count=(variant==0)?
              es.get<age>().count(i%max_age):
              es.get<age>().count(i%steps*step,smaller_decade());
	    if(count<1) // To prevent compiler optimisations.
	      std::cout<<count<<std::endl;
	  }
	  clock_t end1=std::clock();
	  dur1+=end1-start1;

	  clock_t start2=std::clock();
	  for(int i=0;i<iters/loops;++i){
	    int count=(variant==0)?
              ers.get<age>().count(i%max_age):
              ers.get<age>().count(i%steps*step,smaller_decade());
	    if(count<1) // To prevent compiler optimisations.
	      std::cout<<count<<std::endl;
	  }
	  clock_t end2=std::clock();
	  dur2+=end2-start2;

	  // The following is aimed at avoiding the impact of caching.
	  employee_set::iterator begin_index1=es.get<id>().begin();
	  int removed_age1=begin_index1->age;
	  es.get<id>().erase(begin_index1);
	  es.insert(employee(last_id1++,"Anna",removed_age1));

	  employee_ranked_set::iterator begin_index2=ers.get<id>().begin();
	  int removed_age2=begin_index2->age;
	  ers.get<id>().erase(begin_index2);
	  ers.insert(employee(last_id2++,"Anna",removed_age2));
	}

	double durMicro1=((double)dur1)/CLOCKS_PER_SEC*1000;
	std::cout<<std::endl<<std::setw(20)<<durMicro1
		 <<" ms - time of ordered_index.\n";

	double durMicro2=((double)dur2)/CLOCKS_PER_SEC*1000;
	std::cout<<std::setw(20)<<durMicro2
		 <<" ms - time of ranked_index.\n";

	std::cout<<std::fixed<<std::setprecision(2)<<std::setw(20)
		 <<durMicro1/durMicro2<<" - ratio."<<std::endl;
      }
    }
  }
  return 0;
}
