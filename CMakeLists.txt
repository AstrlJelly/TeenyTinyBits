cmake_minimum_required(VERSION 3.31)

project(TeenyTinyBits VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# configure_file(${PROJECT_SOURCE_DIR}/constants.hpp ${PROJECT_BINARY_DIR}/constants.hpp)
# configure_file(${PROJECT_SOURCE_DIR}/constants.hpp.in ${PROJECT_BINARY_DIR}/constants.hpp)
configure_file(${PROJECT_SOURCE_DIR}/constants.hpp.in ${PROJECT_BINARY_DIR}/constants.hpp)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
                           "${PROJECT_SOURCE_DIR}"
                           "${PROJECT_BINARY_DIR}"
                           )