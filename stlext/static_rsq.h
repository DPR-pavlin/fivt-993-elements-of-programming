#ifndef _STATIC_RSQ_H_
#define _STATIC_RSQ_H_

#include <cassert>
#include <functional>
#include <iterator>
#include <numeric>
#include <vector>

#include "safe_distance.h"

namespace stlext {

template<class BinaryOperator>
struct inverse;

template<class T>
struct inverse<std::plus<T> > {
  typedef std::minus<T> value;
};

template<class T>
struct inverse<std::multiplies<T> > {
  typedef std::divides<T> value;
};

template<class T,
         class BinaryOperator = std::plus<T>,
         class InverseOperator = typename inverse<BinaryOperator>::value>
class static_rsq {
 public:
  template<class Iterator>
  static_rsq(Iterator begin, Iterator end,
             const BinaryOperator& binary_operator,
             const InverseOperator& inverse_operator)
      : binary_operator_(binary_operator),
        inverse_operator_(inverse_operator) {
    init(begin, end);
  }

  template<class Iterator>
  static_rsq(Iterator begin, Iterator end) {
    init(begin, end);
  }

  T range_sum(size_t from, size_t to) const {
    assert(from < to);
    assert(to <= partial_sums_.size());

    if (from > 0) {
      return inverse_operator_(partial_sums_[to - 1], partial_sums_[from - 1]);
    } else {
      return partial_sums_[to - 1];
    }
  }

  void push_back(const T& element) {
    if (!partial_sums_.empty()) {
      partial_sums_.push_back(binary_operator_(partial_sums_.back(), element));
    } else {
      partial_sums_.push_back(element);
    }
  }

  void pop_back() {
    assert(!partial_sums_.empty());

    partial_sums_.pop_back();
  }

  size_t size() const {
    return partial_sums_.size();
  }

 private:
  BinaryOperator binary_operator_;
  InverseOperator inverse_operator_;
  std::vector<T> partial_sums_;

  template<class Iterator>
  void init(Iterator begin, Iterator end) {
    partial_sums_.reserve(safe_distance(begin, end));

    std::partial_sum(begin, end,
      std::back_inserter(partial_sums_),
      binary_operator_);
  }
};

}  // namespace stlext

#endif  // _STATIC_RSQ_H_
