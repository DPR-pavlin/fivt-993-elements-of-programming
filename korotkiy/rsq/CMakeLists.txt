cmake_minimum_required(VERSION 2.8)
project(RSQ)

# CMake is awesome!

set (SOURCES
)

set (TESTS
  rsq_unittest.cc
  safe_distance_unittest.cc
)

add_executable (unittests
  ${SOURCES}
  ${TESTS}
)

target_link_libraries (
  unittests
  libgtest.so
  libgtest_main.so
  libpthread.so
)

add_custom_target("Run Unittests" ALL
  ./unittests
  DEPENDS unittests
)