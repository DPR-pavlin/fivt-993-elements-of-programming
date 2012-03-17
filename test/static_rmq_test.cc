#include <gtest/gtest.h>

#include <cstdlib>
#include <algorithm>
#include <functional>
#include <iterator>

#include "static_rmq.h"

namespace stlext {

struct gcd {

  explicit gcd(int a) : fictive(a) {
  }

  gcd() {
  }

  int fictive;

  int operator()(int a, int b) const{
    while (b) {
      a %= b;
      std::swap(a, b);
    }
    return a;
  }
};

struct comparator_with_costructor {

  explicit comparator_with_costructor(int a) : fictive(a) {
  }

  int operator()(const int& a, const int& b) const {
    if (a < b) {
      return a;
    } else {
      return b;
    }
  }

  int fictive;

};

TEST(static_range_idempotent_query_simple, min) {
  int TEST_ARRAY[] = {1, 2, 3, 4, 5, 6};
  size_t SIZE = sizeof(TEST_ARRAY) / sizeof(TEST_ARRAY[0]);
  static_range_idempotent_query<int, min<int> > rmq(TEST_ARRAY,
                                                    TEST_ARRAY + SIZE);
  ASSERT_EQ(1, rmq.query(0, 1));
  ASSERT_EQ(2, rmq.query(1, 4));
  ASSERT_EQ(3, rmq.query(2, 6));
}

TEST(static_range_idempotent_query_simple, gcd) {
  int TEST_ARRAY[] = {1, 2, 4, 8, 6, 9};
  size_t SIZE = sizeof(TEST_ARRAY) / sizeof(TEST_ARRAY[0]);
  static_range_idempotent_query<int, gcd > rmq(TEST_ARRAY, TEST_ARRAY + SIZE);
  ASSERT_EQ(1, rmq.query(0, 1));
  ASSERT_EQ(2, rmq.query(1, 5));
  ASSERT_EQ(3, rmq.query(4, 6));
}

TEST(static_range_idempotent_query_simple, gcd_with_constructor) {
  int TEST_ARRAY[] = {1, 2, 4, 8, 6, 9};
  size_t SIZE = sizeof(TEST_ARRAY) / sizeof(TEST_ARRAY[0]);
  static_range_idempotent_query<int, gcd > rmq(TEST_ARRAY,
                                               TEST_ARRAY + SIZE, gcd(0));
  ASSERT_EQ(1, rmq.query(0, 1));
  ASSERT_EQ(2, rmq.query(1, 5));
  ASSERT_EQ(3, rmq.query(4, 6));
}

TEST(static_rmq_simple, less) {
  int TEST_ARRAY[] = {1, 2, 3, 4, 5, 6};
  size_t SIZE = sizeof(TEST_ARRAY) / sizeof(TEST_ARRAY[0]);
  static_rmq<int> rmq(TEST_ARRAY, TEST_ARRAY + SIZE);
  ASSERT_EQ(1, rmq.query(0, 1));
  ASSERT_EQ(2, rmq.query(1, 4));
  ASSERT_EQ(3, rmq.query(2, 6));
}

TEST(static_rmq_simple, comparator_with_contructor) {
  int TEST_ARRAY[] = {1, 2, 3, 4, 5, 6};
  size_t SIZE = sizeof(TEST_ARRAY) / sizeof(TEST_ARRAY[0]);
  static_rmq<int, comparator_with_costructor> rmq(TEST_ARRAY,
      TEST_ARRAY + SIZE,
      comparator_with_costructor(0));
  ASSERT_EQ(1, rmq.query(0, 1));
  ASSERT_EQ(2, rmq.query(1, 4));
  ASSERT_EQ(3, rmq.query(2, 6));
}

TEST(static_rmq_simple, construction_from_input_iterator) {
  std::istringstream in("1 2 3 4 5 6");
  const size_t SIZE = 6;
  static_rmq<int> rmq(std::istream_iterator<int>(in),
                      (std::istream_iterator<int>()));
  ASSERT_EQ(1, rmq.query(0, 1));
  ASSERT_EQ(2, rmq.query(1, 4));
  ASSERT_EQ(3, rmq.query(2, 6));
}

TEST(static_rmq_simple, greater) {
  int TEST_ARRAY[] = {6, 3, 5, 10, 11, 1, 4, 9};
  size_t SIZE = sizeof(TEST_ARRAY) / sizeof(TEST_ARRAY[0]);
  static_rmq<int, std::greater<int> > rmq(TEST_ARRAY, TEST_ARRAY + SIZE);
  ASSERT_EQ(6, rmq.query(0, 3));
  ASSERT_EQ(11, rmq.query(1, 5));
  ASSERT_EQ(4, rmq.query(5, 7));
  ASSERT_EQ(11, rmq.query(2, 6));
  ASSERT_EQ(11, rmq.query(0, 8));
  ASSERT_EQ(11, rmq.query(3, 8));
}

TEST(static_rmq_stess, less) {
  const int TEST_SIZE = 1000;
  std::vector<int> test_data;
  for (int i = 0; i < TEST_SIZE; ++i) {
    test_data.push_back(std::rand() % 1000000);
  }
  static_rmq<int> rmq(test_data.begin(), test_data.end());
  for (int from = 0; from < TEST_SIZE; from++) {
    for ( int to = from + 1; to <= TEST_SIZE; to++) {
      ASSERT_EQ(*std::min_element(test_data.begin() + from, test_data.begin() + to),
          rmq.query(from, to));
    }
  }
}

} // namespace stlext


