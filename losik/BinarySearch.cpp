#include <functional>
#include <algorithm>


template<class Iterator, class T, class Comparator>
Iterator Find(Iterator begin, Iterator end, Iterator advice, const T& element, Comparator compare) {
	
	if (end - begin <= 0) return end;

	if(end <= advice) advice = end - 1;
	if(advice < begin) advice = begin;

	bool cmp = compare( element, *advice );

	Iterator limit  ( cmp? begin : end );
	Iterator prev_it( cmp? end : begin );
	Iterator next_it( advice );
	
	int sign = cmp? -1 : 1;
	int step = 1;
	
	
	while (sign * (limit - next_it) >= 1 && cmp == compare( element, *next_it )) {
		prev_it = next_it;		
		next_it += sign * step;
		step <<= 1;
	}

	if (cmp) {
		Iterator ans = std::lower_bound(std::max(limit, next_it), prev_it, element, compare);
		if (ans == prev_it) return end;
		else return (*ans == element)? ans : end;				
	}
	else {
		Iterator ans = std::lower_bound(prev_it, std::min(limit, next_it), element, compare);
		if (ans == std::min(limit, next_it)) return end;
		else return (*ans == element)? ans : end;
	}
}

template<class Iterator, class T>
Iterator Find(Iterator begin, Iterator end, Iterator advice, const T& element) {
	return Find<Iterator, T, std::less<T> >(begin, end, advice, element, std::less<T>());
}


