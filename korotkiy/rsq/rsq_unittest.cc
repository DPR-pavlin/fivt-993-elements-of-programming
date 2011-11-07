#include <gtest/gtest.h>

#include <cstdlib>

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>

#include "static_rsq.h"

TEST(StaticRSQSimple, Sum) {
  int TEST_ARRAY[] = {1, 2, 3, 4, 5, 6};
  size_t SIZE = sizeof(TEST_ARRAY) / sizeof(TEST_ARRAY[0]);

  StaticRSQ<int, std::plus<int>, std::minus<int> > rsq(
      TEST_ARRAY, TEST_ARRAY + SIZE);

  ASSERT_EQ(1, rsq.RangeSum(0, 1));
  ASSERT_EQ(6, rsq.RangeSum(SIZE - 1, SIZE));

  ASSERT_EQ(21, rsq.RangeSum(0, SIZE));

  ASSERT_EQ(12, rsq.RangeSum(2, 5));
}

TEST(StaticRSQSimple, Product) {
  int TEST_ARRAY[] = {1, 2, 3, 4, 5, 6};
  size_t SIZE = sizeof(TEST_ARRAY) / sizeof(TEST_ARRAY[0]);

  StaticRSQ<int, std::multiplies<int>, std::divides<int> > rsq(
      TEST_ARRAY, TEST_ARRAY + SIZE);

  ASSERT_EQ(1, rsq.RangeSum(0, 1));
  ASSERT_EQ(6, rsq.RangeSum(SIZE - 1, SIZE));

  ASSERT_EQ(720, rsq.RangeSum(0, SIZE));

  ASSERT_EQ(60, rsq.RangeSum(2, 5));
}

TEST(StaticRSQSimple, ConstructionFromInputIterator) {
  std::istringstream in("1 2 3 4 5 6");
  const size_t SIZE = 6;

  StaticRSQ<int, std::plus<int>, std::minus<int> > rsq(
      std::istream_iterator<int>(in), (std::istream_iterator<int>()));

  ASSERT_EQ(1, rsq.RangeSum(0, 1));
  ASSERT_EQ(6, rsq.RangeSum(SIZE - 1, SIZE));

  ASSERT_EQ(21, rsq.RangeSum(0, SIZE));

  ASSERT_EQ(12, rsq.RangeSum(2, 5));
}

class RangeRandomNumberGenerator {
 public:
  RangeRandomNumberGenerator(int from, int to) : from_(from), to_(to) {}

  int operator() () {
    return std::rand() % (to_ - from_) + from_;
  }

 private:
  int from_, to_;
};

TEST(StaticRSQStress, Product) {
  const int TEST_SIZE = 10;

  std::vector<int> test_data;
  std::generate_n(std::back_inserter(test_data), TEST_SIZE,
                  RangeRandomNumberGenerator(1, 9));

  StaticRSQ<int, std::multiplies<int>, std::divides<int> > rsq(
      test_data.begin(), test_data.end());

  for (int from = 0; from < TEST_SIZE; ++from) {
    for (int to = from + 1; to < TEST_SIZE; ++to) {
      ASSERT_EQ(std::accumulate(test_data.begin() + from,
                                test_data.begin() + to,
                                1, std::multiplies<int>()),
                rsq.RangeSum(from, to));
    }
  }
}
