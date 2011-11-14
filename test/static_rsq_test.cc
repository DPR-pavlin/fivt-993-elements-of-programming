#include <gtest/gtest.h>

#include <cstdlib>

#include <iostream>

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>

#include "static_rsq.h"

namespace stlext {

TEST(static_rsq_simple, sum) {
  int TEST_ARRAY[] = {1, 2, 3, 4, 5, 6};
  size_t SIZE = sizeof(TEST_ARRAY) / sizeof(TEST_ARRAY[0]);

  static_rsq<int> rsq(TEST_ARRAY, TEST_ARRAY + SIZE);

  ASSERT_EQ(1, rsq.range_sum(0, 1));
  ASSERT_EQ(6, rsq.range_sum(SIZE - 1, SIZE));

  ASSERT_EQ(21, rsq.range_sum(0, SIZE));

  ASSERT_EQ(12, rsq.range_sum(2, 5));
}

TEST(static_rsq_simple, product) {
  int TEST_ARRAY[] = {1, 2, 3, 4, 5, 6};
  size_t SIZE = sizeof(TEST_ARRAY) / sizeof(TEST_ARRAY[0]);

  static_rsq<int, std::multiplies<int> > rsq(TEST_ARRAY, TEST_ARRAY + SIZE);

  ASSERT_EQ(1, rsq.range_sum(0, 1));
  ASSERT_EQ(6, rsq.range_sum(SIZE - 1, SIZE));

  ASSERT_EQ(720, rsq.range_sum(0, SIZE));

  ASSERT_EQ(60, rsq.range_sum(2, 5));
}

TEST(static_rsq_simple, construction_from_input_iterator) {
  std::istringstream in("1 2 3 4 5 6");
  const size_t SIZE = 6;

  static_rsq<int> rsq(std::istream_iterator<int>(in),
                     (std::istream_iterator<int>()));

  ASSERT_EQ(1, rsq.range_sum(0, 1));
  ASSERT_EQ(6, rsq.range_sum(SIZE - 1, SIZE));

  ASSERT_EQ(21, rsq.range_sum(0, SIZE));

  ASSERT_EQ(12, rsq.range_sum(2, 5));
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

TEST(static_rsq_stress, product) {
  const int TEST_SIZE = 10;

  std::vector<int> test_data;
  std::generate_n(std::back_inserter(test_data), TEST_SIZE,
                  RangeRandomNumberGenerator(1, 9));

  static_rsq<int, std::multiplies<int> > rsq(test_data.begin(),
                                             test_data.end());

  for (int from = 0; from < TEST_SIZE; ++from) {
    for (int to = from + 1; to < TEST_SIZE; ++to) {
      ASSERT_EQ(std::accumulate(test_data.begin() + from,
                                test_data.begin() + to,
                                1, std::multiplies<int>()),
                rsq.range_sum(from, to));
    }
  }
}

TEST(static_rsq_stress, sum_push_back) {
  const int TEST_SIZE = 50;

  std::vector<int> test_data;
  static_rsq<int> rsq(test_data.begin(), test_data.end());

  for (int size = 0; size < TEST_SIZE; ++size) {
    int random_number = std::rand() % 10000 - 5000;

    test_data.push_back(random_number);
    rsq.push_back(random_number);

    ASSERT_EQ(test_data.size(), rsq.size());

    for (int from = 0; from < rsq.size(); ++from) {
      for (int to = from + 1; to < rsq.size(); ++to) {
        ASSERT_EQ(std::accumulate(test_data.begin() + from,
                                  test_data.begin() + to,
                                  0),
                  rsq.range_sum(from, to));
      }
    }
  }

  do {
    test_data.pop_back();
    rsq.pop_back();

    ASSERT_EQ(test_data.size(), rsq.size());

    for (int from = 0; from < rsq.size(); ++from) {
      for (int to = from + 1; to < rsq.size(); ++to) {
        ASSERT_EQ(std::accumulate(test_data.begin() + from,
                                  test_data.begin() + to,
                                  0),
                  rsq.range_sum(from, to));
      }
    }
  } while (!test_data.empty());
}

}
