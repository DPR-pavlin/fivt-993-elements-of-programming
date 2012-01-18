cmake_minimum_required(VERSION 2.8)

set (STLEXT_LIB_CC
	integer_part_of_log2.cc
)

set (STLEXT_LIB_H
	integer_part_of_log2.h
	safe_distance.h
	static_rmq.h
)

add_library(STLEXT 
	${STLEXT_LIB_H} 
	${STLEXT_LIB_CC}
)