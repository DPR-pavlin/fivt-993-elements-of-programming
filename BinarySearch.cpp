#include <functional>

template<class Iterator, class T, class Comparator>
Iterator binarySearch(Iterator begin, Iterator end, T element, Comparator compare) {
	Iterator middle, no_elem = end;

	while (end - begin > 1) {
		middle = begin + (end - begin) / 2;		
		( compare(element, *middle) ? end  : begin ) = middle;
	}

	return ((*begin == element)? begin : no_elem);
}

template<class T, class Iterator>
Iterator binarySearch(Iterator begin, Iterator end, T element) {
	return binarySearch<Iterator, T, std::less<T> > (begin, end, element, std::less<T>()); 
}


template<class Iterator, class T, class Comparator>
Iterator Find(Iterator begin, Iterator end, Iterator advice, T element, Comparator compare) {
	
	if (end - begin <= 0) return end;

	if(end <= advice) advice = end - 1;
	if(end < begin) advice = begin;

	int step = 1;
	bool cmp = compare( element, *advice );
	int sign = cmp? -1 : 1;
	Iterator limit( cmp? begin : end );
	Iterator it( advice );

	for (; sign * (limit - it) >= 0 && cmp == compare( element, *it ); it += sign*step, step<<1 );

	if (cmp) {
		Iterator ans = binarySearch<Iterator, T, Comparator>(std::max(limit, it), advice, element, compare);
		return (ans == advice)? end : ans;
	}
	else {
		Iterator ans = binarySearch<Iterator, T, Comparator>(advice, std::min(limit, it), element, compare);
		return (ans == std::min(limit, it))? end: ans;
	}
}

template<class Iterator, class T>
Iterator Find(Iterator begin, Iterator end, Iterator advice, T element) {
	return Find<Iterator, T, std::less<T> >(begin, end, advice, element, std::less<T>());
}


