# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "CMakeFiles\\Kodetron_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Kodetron_autogen.dir\\ParseCache.txt"
  "Kodetron_autogen"
  "external\\googletest\\googletest\\CMakeFiles\\gtest_autogen.dir\\AutogenUsed.txt"
  "external\\googletest\\googletest\\CMakeFiles\\gtest_autogen.dir\\ParseCache.txt"
  "external\\googletest\\googletest\\CMakeFiles\\gtest_main_autogen.dir\\AutogenUsed.txt"
  "external\\googletest\\googletest\\CMakeFiles\\gtest_main_autogen.dir\\ParseCache.txt"
  "external\\googletest\\googletest\\gtest_autogen"
  "external\\googletest\\googletest\\gtest_main_autogen"
  "tests\\CMakeFiles\\kodetron_tests_autogen.dir\\AutogenUsed.txt"
  "tests\\CMakeFiles\\kodetron_tests_autogen.dir\\ParseCache.txt"
  "tests\\kodetron_tests_autogen"
  )
endif()
