#ifndef _SAFE_DISTANCE_H_
#define _SAFE_DISTANCE_H_

#include <iterator>

template<class Iterator,
         class IteratorCategory>
class DistanceLowerBound {
 public:
  typedef typename std::iterator_traits<Iterator>::difference_type
          DifferenceType;

  DifferenceType operator() (Iterator begin, Iterator end) {
    return std::distance(begin, end);
  }
};

template<class Iterator>
class DistanceLowerBound<Iterator, std::input_iterator_tag> {
 public:
  typedef typename std::iterator_traits<Iterator>::difference_type
          DifferenceType;

  DifferenceType operator() (Iterator begin, Iterator end) {
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
