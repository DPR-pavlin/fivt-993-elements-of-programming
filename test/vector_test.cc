#include <gtest/gtest.h>

#include <iostream>
#include <vector>
#include <cstdlib>

#include "stlext/vector.h"

TEST(Vector, SimplePushBack) {
  const int TEST_SIZE = 40;
  stlext::vector<int> v;

  for (int i = 0; i < TEST_SIZE; ++i) {
    v.push_back(i);
  }

  for (int i = 0; i < TEST_SIZE; ++i) {
    ASSERT_EQ(i, v[i]);
  }
}

TEST(Vector, ReferenceToElement) {
  const int TEST_SIZE = 40;
  stlext::vector<int> v;

  for (int i = 0; i < TEST_SIZE; ++i) {
    v.push_back(i);
  }

  for (int i = 0; i < TEST_SIZE; ++i) {
    v[i] = TEST_SIZE - i;
  }

  for (int i = 0; i < TEST_SIZE; ++i) {
    ASSERT_EQ(TEST_SIZE - i, v[i]);
  }
}

template<class T>
bool is_vectors_equal(const stlext::vector<T>& our_vector,
                      const std::vector<T>& stl_vector) {
  if (our_vector.size() != stl_vector.size()) {
    return false;
  }

  for (int i = 0; i < stl_vector.size(); ++i) {
    if (our_vector[i] != stl_vector[i]) {
      return false;
    }
  }

  return true;
}

TEST(Vector, RandomAccess) {
  const int VECTOR_SIZE = 50,
            NUMBER_OF_OPERATIONS = 1000;

  stlext::vector<int> our_vector;
  std::vector<int> stl_vector;

  for (int i = 0; i < VECTOR_SIZE; ++i) {
    our_vector.push_back(i);
    stl_vector.push_back(i);

    ASSERT_TRUE(is_vectors_equal(our_vector, stl_vector));
  }

  for (int i = 0; i < NUMBER_OF_OPERATIONS; ++i) {
    size_t index = std::rand() % VECTOR_SIZE,
           value = std::rand();

    our_vector[index] = value;
    stl_vector[index] = value;

    ASSERT_TRUE(is_vectors_equal(our_vector, stl_vector));
  }
}
