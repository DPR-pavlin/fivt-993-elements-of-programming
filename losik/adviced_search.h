#ifndef _ADVICED_SEARCH_H_
#define _ADVICED_SEARCH_H_

#include <iterator>
#include <functional>
#include <utility>
#include "comparator_mutator.h"
#include "find_first_not_less.h"


template <class Iterator, class T, class Comparator>
Iterator adviced_lower_bound(Iterator begin, Iterator end, Iterator advice, const T& element, Comparator compare) {
	if (std::distance(begin, end) <= 0) return end;
	if (std::distance(advice, end) <= 0) advice = end - 1;
	if (std::distance(begin, advice) < 0) advice = begin;


	if (compare(*advice, element)) {
		return find_first_not_less(advice, end, element, compare);
	}
	else {
		return find_first_not_less(
			std::reverse_iterator<Iterator>(advice),
			std::reverse_iterator<Iterator>(begin),
			element,
			create_greater_or_equal(compare)).base();
	}
}

template <class Iterator, class T>
Iterator adviced_lower_bound(Iterator begin, Iterator end, Iterator advice, const T& element) {
	return adviced_lower_bound(begin, end, advice, element, std::less<T>());
}

template <class Iterator, class T, class Comparator>
Iterator adviced_upper_bound(Iterator begin, Iterator end, Iterator advice, const T& element, Comparator compare) {
	return adviced_lower_bound(begin, end, advice, element, create_less_or_equal(compare));
}

template <class Iterator, class T>
Iterator adviced_upper_bound(Iterator begin, Iterator end, Iterator advice, const T& element) {
	return adviced_upper_bound(begin, end, advice, element, std::less<T>());
}

template <class Iterator, class T, class Comparator>
std::pair<Iterator, Iterator> adviced_equal_range(Iterator begin, Iterator end, Iterator advice, const T& element, Comparator compare) {
	Iterator first(begin), last(end);	

	// Checking
	if (begin == end) return std::make_pair(end, end);
	if (std::distance(advice, end) <= 0) advice = (end-1);
	if (std::distance(begin, advice) < 0) begin = advice;
	
	if (compare(*advice, element)) {
		first = adviced_lower_bound(begin, end, advice, element, compare);
		last = adviced_upper_bound(begin, end, first, element, compare);
	} 
	else if (compare(element, *advice)) {
		last = adviced_upper_bound(begin, end, advice, element, compare);
		first = adviced_lower_bound(begin, end, last, element, compare);
	}
	else {
		first = adviced_lower_bound(begin, end, advice, element, compare);
		last = adviced_upper_bound(begin, end, advice, element, compare);
	}
		 	
	return std::make_pair(first, last);
}

template <class Iterator, class T>
std::pair<Iterator, Iterator> adviced_equal_range(Iterator begin, Iterator end, Iterator advice, const T& element) {
	return adviced_equal_range(begin, end, advice, element, std::less<T>());
}

#endif
