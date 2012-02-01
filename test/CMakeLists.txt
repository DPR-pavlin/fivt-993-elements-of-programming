cmake_minimum_required(VERSION 2.8)

include_directories(${CMAKE_SOURCE_DIR}/stlext)


set (TESTS
  integer_part_of_log2_test.cc
  static_rmq_test.cc
)

add_executable (unittests
  ${TESTS}
)



target_link_libraries (
  unittests
  gtest
  gtest_main
  pthread
)

add_test(
  NAME all_unittest
  COMMAND unittests
)