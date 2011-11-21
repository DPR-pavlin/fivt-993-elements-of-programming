#ifndef _FIND_FIRST_NOT_LESS_H_
#define _FIND_FIRST_NOT_LESS_H_

#include <functional>
#include <algorithm>
#include <iterator>
#include <iostream>
#include "comparator_mutator.h"

template<class Iterator, class T, class Comparator>
Iterator find_first_not_less(Iterator begin, Iterator end, const T& element, Comparator less) {
	Iterator it(begin);
	Iterator it_next(it);
	Iterator temp;
	
	greater_or_equal<Comparator> not_less(less);	
	
	typename std::iterator_traits<Iterator>::difference_type step = 1;
	typename std::iterator_traits<Iterator>::difference_type avaliable_distance;


	// Checking for extreme cases	
	if (std::distance(begin, end) <= 0) return end;
	if (not_less(*begin, element)) return begin;
	
	avaliable_distance = std::distance(it_next, end);

	// Detecting interval to search on
	while (avaliable_distance > step) {
		if (not_less(*it_next, element))
			break;

		it = it_next;
		it_next += step;
		avaliable_distance -= step;
		step <<= 1;
	}

	if (less(*it_next, element)) {
		it = it_next;
		it_next = end;
	}


	// Searching on interval
	avaliable_distance = it_next - it;

	while (step != 0) {
		if (avaliable_distance > step) {
			temp = it + step;
			if (less(*temp, element)) {
				it = temp;
				avaliable_distance -= step;
			}
		}
		step >>= 1;
	} 
	
	return ++it;
}

template <class Iterator, class T>
Iterator find_first_not_less(Iterator begin, Iterator end, const T& element) {
	return find_first_not_less(begin, end, element, std::less<T>());
}

#endif
