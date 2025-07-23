cmake_minimum_required(VERSION 3.10)

# Find Qt6 for tests
find_package(Qt6 COMPONENTS Widgets Test REQUIRED)

# Create test executable
add_executable(kodetron_tests
    
)

# Add include directories for the test executable
target_include_directories(kodetron_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/widgets
)

# Link libraries for tests
target_link_libraries(kodetron_tests PRIVATE
    gtest
    gtest_main
    Qt6::Widgets
    Qt6::Test
    qscintilla2_qt6
)

# Disable clang-tidy for tests
set_target_properties(kodetron_tests PROPERTIES CXX_CLANG_TIDY "")

# Add tests to CTest
add_test(NAME FileMenuActionsTest COMMAND kodetron_tests)