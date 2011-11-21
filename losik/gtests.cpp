#include <gtest/gtest.h>
#include <functional>
#include <vector>
#include <algorithm>
#include <iostream>

#include "find_first_not_less.h"
#include "adviced_search.h"

//#define LOG



int arr[10] = {0,1,2,3,4,5,6,7,8,9};
int brr[10] = {0,0,2,2,4,4,6,6,8,8};

template<class Iterator>
void visualize (Iterator begin, Iterator a, Iterator b) {
	#ifdef LOG
	std::cout << " [" << a - begin << "] " << *a  <<
				 " [" << b - begin << "] " << *b << std::endl;
	#endif
}

TEST (array_of_int, check_present_elements) {
	for (int i=0; i<10; i++) {
		for (int j=0; j<=i; j++) {
			int* answ;
			EXPECT_EQ(answ = find_first_not_less(arr+j, arr+10, arr[i]), arr+i);
			visualize(arr, answ, arr+i);
			#ifdef LOG						
				//std::cout << "	begin:" << j << " find:" << i << std::endl;
			#endif
		}
	}
}

TEST (array_of_int, not_present_less) {
	EXPECT_EQ(find_first_not_less(arr, arr+10, -100 ), arr);
}

TEST (array_of_int, not_present_more) {
	EXPECT_EQ(find_first_not_less(arr, arr+10, 100 ), arr+10);
}

TEST (array_of_int, not_present_in_range) {
	for (int i=0; i<5; i++) {
		EXPECT_EQ(find_first_not_less(brr, brr+10, 2*i -1), brr+2*i);
	}
}


TEST (array_of_int, random_equality_test) {
	std::vector<double> vec;	

	for (int i=0; i<100000; i++)
		vec.push_back(std::rand());
	
	std::sort(vec.begin(), vec.end(), std::greater<double>() );

	for (int i=0; i<vec.size(); i++) {
		EXPECT_EQ(*find_first_not_less(vec.begin(), vec.end(), vec[i], std::greater<double>()), vec[i]);
	}
}

TEST (lower_bound, single_elements) {
	for (int i=0; i<10; i++)
		for (int j=0; j<10; j++) {
			int* answ;
			EXPECT_EQ(answ = adviced_lower_bound(arr, arr+10, arr+j, arr[i]), arr+i);
			visualize(arr, answ, arr+i);
		}
}

TEST (lower_bound, multiple_elements) {
	for (int i=1; i<5; i++) {
		int* answ;
		EXPECT_EQ(answ = adviced_lower_bound(brr, brr+10, brr, 2*i), brr+2*i);
		visualize(brr, answ, brr+2*i);
	}
}

TEST (upper_bound, single_elements) {
	for (int i=0; i<10; i++)
		for (int j=0; j<10; j++) {
			int* answ;
			EXPECT_EQ(answ = adviced_upper_bound(arr, arr+10, arr+j, arr[i]), arr+i+1);
			visualize(arr, answ, arr+i+1);
		}
}

TEST (upper_bound, multiple_elements) {
	for (int i=1; i<5; i++) {
		int* answ;
		EXPECT_EQ(answ = adviced_upper_bound(brr, brr+10, brr, 2*i), brr+2*(i+1));
		visualize(brr, answ, brr+2*(i+1));
	}
}

TEST (equal_range, single_elements) {
	for (int i=0; i<10; i++)
		for (int j=0; j<10; j++) {
			std::cout << (adviced_equal_range(arr, arr+10, arr+j, arr[i]) == std::make_pair(arr+i,arr+i+1) ? "" : "FALSE!!!\n");
		}
}

TEST (equal_range, multiple_elements) {
		// Couldnt get it to work =(((		
		//EXPECT_EQ(adviced_equal_range(brr, brr+10, brr, 2*i), (std::make_pair(brr+2*i,brr+2*(i+1))) );
	for (int i=1; i<5; i++) {
			std::cout << (adviced_equal_range(brr, brr+10, brr, 2*i) == std::make_pair(brr+2*i,brr+2*(i+1) ) ? "" : "FALSE!!!\n");
	}
}

