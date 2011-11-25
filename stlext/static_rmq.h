#ifndef _STATIC_RMQ_H_
#define _STATIC_RMQ_H_



#include <cassert>
#include <vector>
#include <algorithm>
#include <cmath>

#include "safe_distance.h"

namespace stlext {


template <class T>
struct min{
	min(){
	}

	T operator() (const T& x, const T& y) const {
			return std::min(x,y);
	}
};

template <class T>
struct max{
	max(){
	}

	T operator() (const T& x, const T& y) const {
		return std::max(x,y);
	}
};


template <class Type,
		class Minimum = min<Type> >
class static_rmq {

public:
	template<class Iterator>
	static_rmq(Iterator begin, Iterator end)
	: minimum_(){
		init(begin, end);
	}

	template<class Iterator>
	  static_rmq(Iterator begin, Iterator end,
	             const Minimum& minimum)
	      : minimum_(minimum) {
	    init(begin, end);
	  }


	Type range_minimun(size_t from, size_t to){
		assert(from < to);
		assert(to <= size_);
		size_t log = logb(to - from);

		//std::cout << "query"<<from << " "<< to<<std::endl;
		//std::cout << log << " " << to - (1>>log) << " "<< (1<<log)<<  std::endl;
		//std::cout << partial_minimum_[log][from] << std::endl;
		//std::cout << partial_minimum_[log][to - (1<<log)] << std::endl;
		return minimum_(partial_minimum_[log][from],
				partial_minimum_[log][to - (1<<log)]);

	}

private:

	std::vector<std::vector<Type> > partial_minimum_;
	size_t size_;
	Minimum minimum_;

	void make_patial_minimum_power_of_2(size_t n){ // n>0

		partial_minimum_[n].reserve(size_ - (1<<n) + 1);
		std::transform( partial_minimum_[n-1].begin(),
				partial_minimum_[n-1].begin() + size_ - (1<<n) + 1,
				partial_minimum_[n-1].begin() + (1<<(n-1)),
				std::back_inserter(partial_minimum_[n]),
				minimum_);

	}

	template<class Iterator>
	void init(Iterator begin, Iterator end){
		partial_minimum_.resize(1);
		partial_minimum_[0].reserve(safe_distance(begin, end));
		partial_minimum_[0].insert(partial_minimum_[0].end(), begin, end);
		size_ = partial_minimum_[0].size();
		size_t log_of_size = logb(size_);
		//std::cout << "log_of_size" << log_of_size << std::endl;
		partial_minimum_.resize(log_of_size + 1);

		for (int i = 1; i <= log_of_size; ++i){
			make_patial_minimum_power_of_2(i);
		}


	}


};


}

#endif //_STATIC_RMQ_H_
