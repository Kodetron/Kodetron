cmake_minimum_required(VERSION 3.10)

# Find Qt6 for tests
find_package(Qt6 COMPONENTS Widgets Test REQUIRED)

# Create test executable
add_executable(kodetron_tests
    test_FileNewAction.cpp
    test_FileOpenAction.cpp
    test_FileSaveAction.cpp
    test_FileSaveAsAction.cpp
    test_FileExitAction.cpp
    # Add the source files that need to be tested
    ../src/widgets/menus/FileMenuActions.cpp
    ../src/widgets/CodeEditor.cpp
    ../src/widgets/LineNumberArea.cpp
)

# Add include directories for the test executable
target_include_directories(kodetron_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/widgets/menus
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/widgets/toolbar
)

# Link libraries for tests
target_link_libraries(kodetron_tests PRIVATE
    gtest
    gtest_main
    Qt6::Widgets
    Qt6::Test
)

# Disable clang-tidy for tests
set_target_properties(kodetron_tests PROPERTIES CXX_CLANG_TIDY "")

# Add tests to CTest
add_test(NAME FileMenuActionsTest COMMAND kodetron_tests)