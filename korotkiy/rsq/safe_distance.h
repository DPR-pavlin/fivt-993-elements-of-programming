#ifndef _SAFE_DISTANCE_H_
#define _SAFE_DISTANCE_H_

#include <iterator>

template<class Iterator,
         class IteratorCategory>
class DistanceLowerBound {
 public:
  typename std::iterator_traits<Iterator>::difference_type
  operator() (Iterator begin, Iterator end) const {
    return std::distance(begin, end);
  }
};

template<class Iterator>
class DistanceLowerBound<Iterator, std::input_iterator_tag> {
 public:
  typename std::iterator_traits<Iterator>::difference_type
  operator() (Iterator begin, Iterator end) const {
    return 0;
  }
};

template<class Iterator>
typename std::iterator_traits<Iterator>::difference_type
SafeDistance(Iterator begin, Iterator end) {
  return DistanceLowerBound<Iterator,
      typename std::iterator_traits<Iterator>::iterator_category>()
          (begin, end);
}

#endif  // _SAFE_DISTANCE_H_
