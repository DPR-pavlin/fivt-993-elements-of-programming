#include <gtest/gtest.h>

#include <cmath>


#include "integer_part_of_log2.h"

namespace stlext {


TEST(integer_part_of_log2, check){
	for (size_t i=1; i<100000000; i++ ){
		ASSERT_EQ(integer_part_of_log2(i), logb(i));
	}
}

} // namespace stlext
