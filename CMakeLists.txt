cmake_minimum_required(VERSION 3.31)

project(TeenyTinyBits VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(${PROJECT_SOURCE_DIR}/scripts/constants.hpp.in ${PROJECT_BINARY_DIR}/constants.hpp)

add_executable(${PROJECT_NAME} scripts/main.cpp include/GLAD/glad.c )

add_subdirectory(include/GLFW)
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw OpenGL::GL)

target_include_directories(${PROJECT_NAME} PUBLIC
                           "${PROJECT_SOURCE_DIR}/scripts"
                           "${PROJECT_SOURCE_DIR}/include/"
                           "${PROJECT_BINARY_DIR}"
                           )