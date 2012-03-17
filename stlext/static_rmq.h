#ifndef _STATIC_RMQ_H_
#define _STATIC_RMQ_H_

#include <cassert>
#include <vector>
#include <algorithm>
#include <cmath>
#include <functional>

#include "safe_distance.h"
#include "integer_part_of_log2.h"

namespace stlext {



/*
 *This class implements a static data structure which holds interval and allows
 *to query value of an idempotent associative function (e.g. minimum, maximum,
 *greatest common divisor, lowest common ancestor) at subintervals.
 *
 *Complexity (N - size of interval):
 *  Preprocessing: O(N log N)
 *  Query: O(1)
 *  Memory: O(N log N)
 */

template<class T, class Idempotent_Operation>
class static_range_idempotent_query {

 public:
  template<class Iterator>
  static_range_idempotent_query(Iterator begin, Iterator end) {
    init(begin, end);
  }

  template<class Iterator>
  static_range_idempotent_query(Iterator begin, Iterator end,
      const Idempotent_Operation& idempotent_operation)
      : idempotent_operation_(idempotent_operation) {
    init(begin, end);
  }

  T query(size_t from, size_t to) const{
    assert(from < to);
    assert(to <= size_);
    size_t log = integer_part_of_log2(to - from);
    const std::vector<T>& log_row = sparse_table_[log];
    return idempotent_operation_(log_row[from],
                                 log_row[to - (1 << log)]);
  }

 private:
  std::vector<std::vector<T> > sparse_table_;
  size_t size_;
  Idempotent_Operation idempotent_operation_;

  void make_sparse_table(size_t n) { // n > 0
    sparse_table_[n].reserve(size_ - (1 << n) + 1);
    std::transform(sparse_table_[n - 1].begin(),
                   sparse_table_[n - 1].begin() + size_ - (1 << n) + 1,
                   sparse_table_[n - 1].begin() + (1 << (n - 1)),
                   std::back_inserter(sparse_table_[n]),
                   idempotent_operation_);
  }

  template<class Iterator>
  void init(Iterator begin, Iterator end) {
    sparse_table_.resize(1);
    sparse_table_[0].reserve(safe_distance(begin, end));
    sparse_table_[0].insert(sparse_table_[0].end(), begin, end);
    size_ = sparse_table_[0].size();
    size_t log_of_size = integer_part_of_log2(size_);
    sparse_table_.resize(log_of_size + 1);
    for (int i = 1; i <= log_of_size; ++i) {
      make_sparse_table(i);
    }
  }

};



/*
  This class creates minimum for comparator.
*/

template<class T, class Comparator = std::less<T> >
class min {
 public:
  min() {
  }

  min(const Comparator& cmp)
      : cmp_(cmp) {
  }

  T operator()(const T& x, const T& y) const {
    if (cmp_(x, y)) {
      return x;
    } else {
      return y;
    }
  }

 private:
  Comparator cmp_;

};


/*
 * This class is implementation of the same data structure in the case when the
 * idempotent associative function is the minimum for some comparator.
 */

template<class T, class Comparator = std::less<T> >
class static_rmq:
  public static_range_idempotent_query<T, min<T, Comparator> > {

 public:
  template<class Iterator>
  static_rmq(Iterator begin, Iterator end)
      : static_range_idempotent_query<T, min<T, Comparator> > (begin, end) {
  }

  template<class Iterator>
  static_rmq(Iterator begin, Iterator end, const Comparator& cmp)
      : static_range_idempotent_query<T, min<T, Comparator> > (begin, end,
          min<T,Comparator> (cmp)) {
  }

};

} // namespace stlext

#endif //_STATIC_RMQ_H_
