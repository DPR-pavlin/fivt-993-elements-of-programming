#include <gtest/gtest.h>

#include <vector>
#include <list>
#include <sstream>

#include "safe_distance.h"

namespace stlext {

TEST(safe_distance, vector_iterators) {
  const int LENGTH = 100;
  std::vector<int> test_vector(LENGTH, 0);

  ASSERT_EQ(LENGTH, safe_distance(test_vector.begin(), test_vector.end()));
}

TEST(safe_distance, list_iterators) {
  const int LENGTH = 100;
  std::list<int> test_list(LENGTH, 0);

  ASSERT_EQ(LENGTH, safe_distance(test_list.begin(), test_list.end()));
}

TEST(safe_distance, input_iterators) {
  std::istringstream stream("1 2 3 4 5");

  ASSERT_EQ(0, safe_distance(
      std::istream_iterator<int>(stream),
      (std::istream_iterator<int>())));
}

}
