#include <gtest/gtest.h>
#include "BinarySearch.cpp"

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <vector>

const int ARR_SIZE = 3000;
const int SRAND_INIT = 12345;
int arr[ARR_SIZE];

int main(int argc, char **argv) {
	std::srand(SRAND_INIT);
	::testing::InitGoogleTest(&argc, argv);
  
	return RUN_ALL_TESTS();
}

TEST(find_element_with_any_advice, simple_random) {
	for (int i=0; i<ARR_SIZE; i++)
		arr[i] = std::rand() % ARR_SIZE;
	
	std::sort(arr, arr + ARR_SIZE);

	for (int i=0; i<ARR_SIZE; i++)
		for (int j=0; j<ARR_SIZE; j++)
			ASSERT_EQ(*Find(arr, arr+ARR_SIZE, arr+j, arr[i]), arr[i]);
}

TEST(find_element_with_any_advice, finds_element_with_different_comparator) {
	for (int i=0; i<ARR_SIZE; i++)
		arr[i] = std::rand() % ARR_SIZE;

	std::sort(arr, arr + ARR_SIZE, std::greater<int>());

	for (int i=0; i<ARR_SIZE; i++)
		for (int j=0; j<ARR_SIZE; j++) {
			ASSERT_EQ(*Find(arr, arr+ARR_SIZE, arr+j, arr[i], std::greater<int>()), arr[i]);
		}
}

TEST(find_element_with_any_advice, different_comparator_different_interval_1) {
	for (int i=0; i<ARR_SIZE; i++)
		arr[i] = std::rand() % ARR_SIZE;

	std::sort(arr, arr + ARR_SIZE, std::greater<int>());

	for (int i=0; i<ARR_SIZE/2; i++)
		for (int j=0; j<ARR_SIZE/2; j++)
			ASSERT_EQ(*Find(arr, arr+ARR_SIZE/2, arr+j, arr[i], std::greater<int>()), arr[i]);
}

TEST(find_element_with_any_advice, different_interval) {
	for (int i=0; i<ARR_SIZE; i++)
		arr[i] = std::rand() % ARR_SIZE;

	std::sort(arr, arr + ARR_SIZE);

	for (int i=ARR_SIZE/2; i<ARR_SIZE; i++)
		for (int j=ARR_SIZE/2; j<ARR_SIZE; j++)
			ASSERT_EQ(*Find(&arr[ARR_SIZE/2], arr+ARR_SIZE, arr+j, arr[i]), arr[i]);
}

TEST(find_element_with_any_advice, different_comparator_different_interval_2) {
	for (int i=0; i<ARR_SIZE; i++)
		arr[i] = std::rand() % ARR_SIZE;

	std::sort(arr, arr + ARR_SIZE, std::greater<int>());

	for (int i=ARR_SIZE/4; i<ARR_SIZE/2; i++)
		for (int j=ARR_SIZE/4; j<ARR_SIZE/2; j++)
			ASSERT_EQ(*Find(&arr[ARR_SIZE/4], arr+ARR_SIZE/2, arr+j, arr[i], std::greater<int>()), arr[i]);
}


/** TEST GROUP no such element is present **/

TEST(no_such_element_present, even_and_odd) {
	for (int i=0; i<ARR_SIZE; i++)
		arr[i] = 2 * (std::rand() % ARR_SIZE);

	std::sort(arr, arr + ARR_SIZE);

	for (int i=0; i<ARR_SIZE; i++)
		for (int j=0; j<ARR_SIZE; j++)
			ASSERT_EQ(Find(arr, arr+ARR_SIZE, arr+j, 2*i + 1), arr+ARR_SIZE);
}

TEST(no_such_element_present, out_of_search_range_1) {
	for (int i=0; i<ARR_SIZE; i++)
		arr[i] = (std::rand() % ARR_SIZE);

	std::sort(arr, arr + ARR_SIZE);

	int i= ARR_SIZE/2;
	for (; i<ARR_SIZE; i++)
		if (arr[i] != arr[i-1]) break;

	ASSERT_EQ(Find(arr, arr+i, arr+i/2, arr[i]), arr+i);
}

TEST(edges, zero_sized) {
	ASSERT_EQ(Find(arr+100, arr+100, arr+100, 12345), arr+100);
}

TEST(edges, advice_out_of_interveal_1) {
	ASSERT_EQ(*Find(arr+100, arr+200, arr+201, arr[150]), arr[150]);
}

TEST(edges, advice_out_of_interveal_2) {
	ASSERT_EQ(*Find(arr+100, arr+200, arr+10, arr[150]), arr[150]);
}

TEST(edges, advice_out_of_range_1) {
	ASSERT_EQ(*Find(arr+100, arr+200, arr+ARR_SIZE*2, arr[150]), arr[150]);
}

TEST(edges, advice_out_of_range_2) {
	ASSERT_EQ(*Find(arr+100, arr+200, arr-2, arr[150]), arr[150]);
}

TEST(edges, empty) {
	int* nll = 0;
	ASSERT_EQ(Find(nll,nll,nll,100), nll);
}

TEST(edges, wrong_border_but_correct_advice_1) {
	ASSERT_EQ(*Find(arr+ARR_SIZE/4, arr+2*ARR_SIZE, arr+ARR_SIZE, arr[ARR_SIZE/2]), arr[ARR_SIZE/2]);
}

TEST(edges, wrong_border_but_correct_advice_2) {
	ASSERT_EQ(*Find(arr-ARR_SIZE/4, arr+ARR_SIZE, arr, arr[ARR_SIZE/2]), arr[ARR_SIZE/2]);
}

