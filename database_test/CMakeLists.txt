# Database Test CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

project(DatabaseTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Sql REQUIRED)

qt_standard_project_setup()

set(DATABASE_TEST_SOURCES
    ../src/database/DatabaseTest.cpp
    ../src/database/DatabaseManager.cpp
)

set(DATABASE_TEST_HEADERS
    ../src/database/DatabaseManager.h
)

add_executable(DatabaseTest ${DATABASE_TEST_SOURCES} ${DATABASE_TEST_HEADERS})

target_include_directories(DatabaseTest PRIVATE
    ../src/database
)

target_link_libraries(DatabaseTest PRIVATE Qt6::Core Qt6::Sql)

# Copy database schema to build directory
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../database/schema.sql 
               ${CMAKE_CURRENT_BINARY_DIR}/database/schema.sql COPYONLY) 