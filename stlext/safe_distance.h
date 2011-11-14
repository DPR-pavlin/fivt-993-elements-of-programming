#ifndef _SAFE_DISTANCE_H_
#define _SAFE_DISTANCE_H_

#include <iterator>

namespace stlext {

template<class Iterator,
         class IteratorCategory>
struct safe_distance_t {
  typename std::iterator_traits<Iterator>::difference_type
  operator() (Iterator begin, Iterator end) const {
    return std::distance(begin, end);
  }
};

template<class Iterator>
struct safe_distance_t<Iterator, std::input_iterator_tag> {
  typename std::iterator_traits<Iterator>::difference_type
  operator() (Iterator begin, Iterator end) const {
    return 0;
  }
};

template<class Iterator>
typename std::iterator_traits<Iterator>::difference_type
safe_distance(Iterator begin, Iterator end) {
  return safe_distance_t<Iterator,
      typename std::iterator_traits<Iterator>::iterator_category>()
          (begin, end);
}

}

#endif  // _SAFE_DISTANCE_H_
