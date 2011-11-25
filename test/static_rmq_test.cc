#include <gtest/gtest.h>

#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <functional>
#include <iterator>

#include "static_rmq.h"


namespace stlext {


TEST(static_rmq_simple1, min) {
	int TEST_ARRAY[] = {1, 2, 3, 4, 5, 6};
	size_t SIZE = sizeof(TEST_ARRAY) / sizeof(TEST_ARRAY[0]);


	static_rmq<int, min<int> > rmq(TEST_ARRAY, TEST_ARRAY + SIZE);


	ASSERT_EQ(1, rmq.range_minimun(0, 1));
	ASSERT_EQ(2, rmq.range_minimun(1, 4));
	ASSERT_EQ(3, rmq.range_minimun(2, 6));


}

TEST(static_rmq_simple1, construction_from_input_iterator) {
	std::istringstream in("1 2 3 4 5 6");
	const size_t SIZE = 6;

	static_rmq<int, min<int> > rmq(std::istream_iterator<int>(in),
                     (std::istream_iterator<int>()));

	ASSERT_EQ(1, rmq.range_minimun(0, 1));
	ASSERT_EQ(2, rmq.range_minimun(1, 4));
  	ASSERT_EQ(3, rmq.range_minimun(2, 6));
}


TEST(static_rmq_simple2, min) {
	int TEST_ARRAY[] = {6, 3, 5, 10, 11, 1, 4, 9};
	size_t SIZE = sizeof(TEST_ARRAY) / sizeof(TEST_ARRAY[0]);


	static_rmq<int, min<int> > rmq(TEST_ARRAY, TEST_ARRAY + SIZE);


	ASSERT_EQ(3, rmq.range_minimun(0, 3));
	ASSERT_EQ(3, rmq.range_minimun(1, 5));
	ASSERT_EQ(1, rmq.range_minimun(5, 7));
	ASSERT_EQ(1, rmq.range_minimun(2, 6));
	ASSERT_EQ(1, rmq.range_minimun(0, 8));
	ASSERT_EQ(1, rmq.range_minimun(3, 8));

}

TEST(static_rmq_simple2, max) {
	int TEST_ARRAY[] = {6, 3, 5, 10, 11, 1, 4, 9};
	size_t SIZE = sizeof(TEST_ARRAY) / sizeof(TEST_ARRAY[0]);


	static_rmq<int, max<int> > rmq(TEST_ARRAY, TEST_ARRAY + SIZE);


	ASSERT_EQ(6, rmq.range_minimun(0, 3));
	ASSERT_EQ(11, rmq.range_minimun(1, 5));
	ASSERT_EQ(4, rmq.range_minimun(5, 7));
	ASSERT_EQ(11, rmq.range_minimun(2, 6));
	ASSERT_EQ(11, rmq.range_minimun(0, 8));
	ASSERT_EQ(11, rmq.range_minimun(3, 8));

}

TEST(static_rmq_stess, min) {
	const int TEST_SIZE = 1000;

	std::vector<int> test_data;
	for (int i  = 0; i < TEST_SIZE; ++i) {
		test_data.push_back(std::rand() % 1000000);
	}
	static_rmq<int, min<int> > rmq(test_data.begin(), test_data.end());

	for (int from = 0; from < TEST_SIZE; from++){
		for ( int  to = from + 1; to<=TEST_SIZE; to++){

			ASSERT_EQ(*std::min_element(test_data.begin() + from, test_data.begin() + to),
					rmq.range_minimun(from, to));
		}
	}


}


}
