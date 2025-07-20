# Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Project name
project(Kodetron VERSION 0.1.0)

# Enable C++17 standard or higher
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt modules
find_package(Qt6 COMPONENTS Widgets Sql Network REQUIRED)

# Try to find Crow (optional for backend server)
find_package(Crow QUIET)

# If using Qt6, set up the module paths
if (TARGET Qt6::Widgets)
    qt_standard_project_setup() # Sets up includes, defines for Qt6
    # Optional: Enable C++ standard if not already done by qt_standard_project_setup()
    # target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
endif()

# Include all source files and headers from the src directory and its subdirectories
file(GLOB KODETRON_SOURCES
    *.cpp
    src/*.cpp
    src/*/*.cpp
    src/*/*/*.cpp
    src/*/*/*/*.cpp
    src/*/*/*/*/*.cpp
    src/*/*/*/*/*/*/*.cpp
)
file(GLOB KODETRON_HEADERS
    *.h
    src/*.h
    src/*/*.h
    src/*/*/*.h
    src/*/*/*/*.h
    src/*/*/*/*/*.h
    src/*/*/*/*/*/*/*.h
)

# Add your executable, including all source files
add_executable(${PROJECT_NAME} ${KODETRON_SOURCES} ${KODETRON_HEADERS}) # Include headers for MOC processing

# Add the directory containing your custom headers to the include paths
# This tells the compiler to look in 'Kodetron/src/widgets' for headers like "CodeEditor.h"
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/menus
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/menu_section
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/toolbar
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DataBase
)

# Link Qt libraries
if (TARGET Qt6::Widgets)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Sql Qt6::Network)
    # Qt6 also needs to set rpath for executables for deployment
    # set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH_USE_LINK_PATH}")
    # set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()

# Clang-tidy configuration
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")

if(CLANG_TIDY_EXE)
    message(STATUS "Found clang-tidy: ${CLANG_TIDY_EXE}")
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};--extra-arg=-std=c++20")
endif()

# Clang-format configuration
find_program(CLANG_FORMAT_EXE NAMES "clang-format")

if(CLANG_FORMAT_EXE)
    message(STATUS "Found clang-format: ${CLANG_FORMAT_EXE}")
endif()


# GoogleTest Integration
# Only add if testing is enabled
enable_testing()

# Disable Google Mock if not needed
set(gmock_build_tests OFF CACHE BOOL "Disable gmock tests" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "Disable gmock build" FORCE)

add_subdirectory(external/googletest)

# Disable clang-tidy for GTest and GMock
set_target_properties(gtest PROPERTIES CXX_CLANG_TIDY "")
set_target_properties(gtest_main PROPERTIES CXX_CLANG_TIDY "")
if (TARGET gmock)
    set_target_properties(gmock PROPERTIES CXX_CLANG_TIDY "")
endif()

if (TARGET gmock_main)
    set_target_properties(gmock_main PROPERTIES CXX_CLANG_TIDY "")
endif()
add_subdirectory(tests)

# Optional: Build backend server if Crow is available
if(Crow_FOUND)
    message(STATUS "Crow found - building backend server")
    add_subdirectory(backend)
else()
    message(STATUS "Crow not found - skipping backend server build")
endif()
