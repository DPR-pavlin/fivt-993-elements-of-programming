#ifndef _STATIC_RSQ_H_
#define _STATIC_RSQ_H_

#include <iterator>
#include <numeric>
#include <vector>

template<class T, class BinaryOperator, class InverseOperator>
class StaticRSQ {
 public:
  template<class Iterator, class IteratorTag>
  void reserve(Iterator begin, Iterator end, IteratorTag) {
    partial_sums_.reserve(std::distance(begin, end));
  }

  template<class Iterator>
  void reserve(Iterator begin, Iterator end, std::input_iterator_tag) {}

  template<class Iterator>
  StaticRSQ(Iterator begin, Iterator end) {
    reserve(begin, end,
        typename std::iterator_traits<Iterator>::iterator_category());
    std::partial_sum(begin, end,
      std::back_inserter(partial_sums_),
      binary_operator_);
  }

  T RangeSum(size_t from, size_t to) {
    if (from > 0) {
      return inverse_operator_(partial_sums_[to - 1], partial_sums_[from - 1]);
    } else {
      return partial_sums_[to - 1];
    }
  }

 private:
  BinaryOperator binary_operator_;
  InverseOperator inverse_operator_;
  std::vector<T> partial_sums_;
};

#endif  // _STATIC_RSQ_H_
