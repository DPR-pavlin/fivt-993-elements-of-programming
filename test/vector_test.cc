#include <gtest/gtest.h>

#include <iostream>

#include "stlext/vector.h"

TEST(Vector, ISuckAtTesting) {
  const int TEST_SIZE = 40;
  stlext::vector<int> v;

  for (int i = 0; i < TEST_SIZE; ++i) {
    v.push_back(i);
  }

  for (int i = 0; i < TEST_SIZE; ++i) {
    ASSERT_EQ(i, v[i]);
  }
}
