#ifndef _RSQ_H_
#define _RSQ_H_

#include <algorithm>
#include <iterator>
#include <numeric>
#include <vector>

template<class T, class BinaryOperator, class InverseOperator>
class StaticRSQ {
 public:
  template<class InputIterator>
  StaticRSQ(InputIterator begin, InputIterator end) {
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

#endif  // _RSQ_H_
