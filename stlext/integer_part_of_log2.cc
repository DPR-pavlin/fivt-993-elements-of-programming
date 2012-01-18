#include "integer_part_of_log2.h"

#include <limits>
#include <cstddef>

namespace stlext {

size_t integer_part_of_log2(size_t number){
	size_t power = 0;
	size_t tmp;
	for (size_t i = std::numeric_limits<size_t>::digits >> 1; i > 0; i >>= 1) {
		tmp = number >> i;
		if (tmp > 0) {
			number = tmp;
			power += i;
		}
	}
	return power;
}


}
