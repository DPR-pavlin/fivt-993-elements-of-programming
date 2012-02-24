#include <gtest/gtest.h>

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>

#include "persistent_heap.h"

namespace stlext {

TEST(persistent_heap, simple) {
  persistent_heap<int> h;
  int size = 20;
  for (size_t i = 0; i < size; i++){
    //std::cout << "ok" << std::endl;
    h.push(i);
  }
  for (size_t i = 0; i < size; i++){
    //std::cout << h.minimum() << std::endl;
    ASSERT_EQ(i, h.pop());
  }
  for (size_t i = size; i > 0; i--){
    h.push(i);
  }
  for (size_t i = 1; i <= size; i++){
    ASSERT_EQ(i, h.pop());
  }
}



TEST(persistent_heap, random_vector) {
  persistent_heap<int> h;
  std::vector<int> v;
  int size = 100;
  for (size_t i = 0; i<size; i++){
     v.push_back(i);
  }
  random_shuffle (v.begin(), v.end());
  for( auto i : v ) {
    h.push(i);
  }
  for(size_t i = 0; i<size; i++) {
    ASSERT_EQ(i, h.pop());
  }
}



TEST(persistent_heap, stress_random_vector) {
  int size = 10000;
  for (size_t  j = 0; j < 100; j++) {
    if (j > 10) {
      size = 1000;
    }
    std::srand(j);
    persistent_heap<int> h;
    std::vector<int> v;
    for (size_t i = 0; i<size; i++){
       v.push_back(i);
    }
    random_shuffle (v.begin(), v.end());
    for( auto i : v ) {
      h.push(i);
    }
    for(size_t i = 0; i<size; i++) {
      ASSERT_EQ(i, h.pop());
    }
  }
}




TEST(persistent_heap, simple_constructor_and_operator) {
  persistent_heap<int> h1;
  h1.push(5);
  persistent_heap<int> h2(h1);
  h2.push(4);
  h2.push(6);

  persistent_heap<int> h3 = h2;
  ASSERT_EQ(4, h3.pop());

  h3 = h1;
  ASSERT_EQ(5, h3.pop());
  ASSERT_EQ(1, h3.empty());

  ASSERT_EQ(5, h1.pop());
  ASSERT_EQ(1, h1.empty());

  ASSERT_EQ(4, h2.pop());
  ASSERT_EQ(5, h2.pop());
  ASSERT_EQ(6, h2.pop());
  ASSERT_EQ(1, h2.empty());

}



}
