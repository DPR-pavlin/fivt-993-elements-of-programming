#include <gtest/gtest.h>

#include <vector>
#include <list>
#include <sstream>

#include "safe_distance.h"

TEST(DistanceLowerBound, VectorIterators) {
  const int LENGTH = 100;
  std::vector<int> test_vector(LENGTH, 0);

  ASSERT_EQ(LENGTH, SafeDistance(test_vector.begin(), test_vector.end()));
}

TEST(DistanceLowerBound, ListIterators) {
  const int LENGTH = 100;
  std::list<int> test_list(LENGTH, 0);

  ASSERT_EQ(LENGTH, SafeDistance(test_list.begin(), test_list.end()));
}

TEST(DistanceLowerBound, InputIterators) {
  std::istringstream stream("1 2 3 4 5");

  ASSERT_EQ(0, SafeDistance(
      std::istream_iterator<int>(stream),
      (std::istream_iterator<int>())));
}
