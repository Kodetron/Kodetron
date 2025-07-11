# Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Project name
project(Kodetron VERSION 0.1.0)

# Enable C++17 standard or higher
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt modules
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# If using Qt6, set up the module paths
if (TARGET Qt6::Widgets)
    qt_standard_project_setup() # Sets up includes, defines for Qt6
    # Optional: Enable C++ standard if not already done by qt_standard_project_setup()
    # target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
endif()

# Find Boost
set(BOOST_ROOT "C:/Boost/Libraries")
find_package(Boost 1.88 COMPONENTS system filesystem REQUIRED)

# Add include directories
include_directories(${Boost_INCLUDE_DIRS})

# Define all source files for your project
set(KODETRON_SOURCES
    src/main.cpp
    src/widgets/CodeEditor.cpp
    src/widgets/LineNumberArea.cpp
    src/widgets/menus/FileMenuActions.cpp
    src/widgets/toolbar/ToolbarManager.cpp
)

# Define all header files for your project (optional, but good for clarity)
set(KODETRON_HEADERS
    src/widgets/CodeEditor.h
    src/widgets/LineNumberArea.h
    src/widgets/menus/FileMenuActions.h
    src/widgets/toolbar/ToolbarManager.h
)

# Add your executable, including all source files
add_executable(${PROJECT_NAME} ${KODETRON_SOURCES} ${KODETRON_HEADERS}) # Include headers for MOC processing

# Add the directory containing your custom headers to the include paths
# This tells the compiler to look in 'Kodetron/src/widgets' for headers like "CodeEditor.h"
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/menus
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/toolbar
)

# Link Qt libraries
if (TARGET Qt6::Widgets)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
    # Qt6 also needs to set rpath for executables for deployment
    # set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH_USE_LINK_PATH}")
    # set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()


# Link Boost libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})