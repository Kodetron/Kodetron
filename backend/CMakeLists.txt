cmake_minimum_required(VERSION 3.10)

# Find required packages
find_package(Qt6 COMPONENTS Core Sql REQUIRED)
find_package(Crow REQUIRED)

# Include nlohmann/json for JSON serialization
find_package(nlohmann_json QUIET)
if(NOT nlohmann_json_FOUND)
    # If not found, we'll use a simpler JSON approach
    message(STATUS "nlohmann/json not found - using simple JSON serialization")
endif()

# Backend server executable
add_executable(kodetron_server
    main_server.cpp
    controllers/UsersController.cpp
    controllers/ThemesController.cpp
    controllers/SnippetsController.cpp
    controllers/TemplatesController.cpp
    controllers/ContestsController.cpp
    controllers/CodeforcesAccountsController.cpp
    middleware/AuthMiddleware.cpp
    utils/JsonSerializer.cpp
    ../src/DataBase/DatabaseManager.cpp
    ../src/DataBase/Models.cpp
    ../src/DataBase/UserRepository.cpp
    ../src/DataBase/ThemeRepository.cpp
    ../src/DataBase/DatabaseService.cpp
)

# Include directories
target_include_directories(kodetron_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/controllers
    ${CMAKE_CURRENT_SOURCE_DIR}/middleware
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/DataBase
)

# Link libraries
target_link_libraries(kodetron_server PRIVATE
    Qt6::Core
    Qt6::Sql
    Crow::Crow
)

# Link nlohmann/json if available
if(nlohmann_json_FOUND)
    target_link_libraries(kodetron_server PRIVATE nlohmann_json::nlohmann_json)
    target_compile_definitions(kodetron_server PRIVATE HAS_NLOHMANN_JSON)
endif()

# Set C++ standard
target_compile_features(kodetron_server PRIVATE cxx_std_17)

# Disable clang-tidy for backend
set_target_properties(kodetron_server PROPERTIES CXX_CLANG_TIDY "")
