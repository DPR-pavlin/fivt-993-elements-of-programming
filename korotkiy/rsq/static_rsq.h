#ifndef _STATIC_RSQ_H_
#define _STATIC_RSQ_H_

#include <cassert>
#include <functional>
#include <iterator>
#include <numeric>
#include <vector>

#include "safe_distance.h"

template<class T, class BinaryOperator>
class Inverse;

template<class T>
class Inverse<T, std::plus<T> > : public std::minus<T> {};

template<class T>
class Inverse<T, std::multiplies<T> > : public std::divides<T> {};

template<class T,
         class BinaryOperator = std::plus<T>,
         class InverseOperator = Inverse<T, BinaryOperator> >
class StaticRSQ {
 public:
  template<class Iterator>
  StaticRSQ(Iterator begin, Iterator end,
            const BinaryOperator& binary_operator = BinaryOperator(),
            const InverseOperator& inverse_operator = InverseOperator())
      : binary_operator_(binary_operator),
        inverse_operator_(inverse_operator) {
    partial_sums_.reserve(SafeDistance(begin, end));

    std::partial_sum(begin, end,
      std::back_inserter(partial_sums_),
      binary_operator_);
  }

  T RangeSum(size_t from, size_t to) const {
    assert(from < to);
    assert(0 <= from);
    assert(to <= partial_sums_.size());

    if (from > 0) {
      return inverse_operator_(partial_sums_[to - 1], partial_sums_[from - 1]);
    } else {
      return partial_sums_[to - 1];
    }
  }

  void PushBack(const T& element) {
    partial_sums_.push_back(binary_operator_(partial_sums_.back(), element));
  }

  void PopBack() {
    assert(!partial_sums_.empty());

    partial_sums_.pop_back();
  }

 private:
  BinaryOperator binary_operator_;
  InverseOperator inverse_operator_;
  std::vector<T> partial_sums_;
};

#endif  // _STATIC_RSQ_H_
