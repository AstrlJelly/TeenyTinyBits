cmake_minimum_required(VERSION 3.31)

project(TeenyTinyBits VERSION 0.2.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(PROJECT_SCRIPTS_DIR "${PROJECT_SOURCE_DIR}/src")
set(PROJECT_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

configure_file("${PROJECT_SCRIPTS_DIR}/constants.hpp.in" "${PROJECT_BINARY_DIR}/constants.hpp")


file(GLOB_RECURSE PROJECT_SCRIPTS
    "${PROJECT_SCRIPTS_DIR}/*.cpp"
)
message(NOTICE "${PROJECT_SCRIPTS}")

file(GLOB IMGUI_SCRIPTS
    "${PROJECT_INCLUDE_DIR}/ImGui/*.cpp"
    "${PROJECT_INCLUDE_DIR}/ImGui/backends/imgui_impl_glfw.cpp"
    "${PROJECT_INCLUDE_DIR}/ImGui/backends/imgui_impl_opengl3.cpp"
    # "${PROJECT_INCLUDE_DIR}/ImGui/backends/*.cpp"
)

add_executable(${PROJECT_NAME}
    "${PROJECT_SCRIPTS}"
    "${IMGUI_SCRIPTS}"
    
    # glad
    "${PROJECT_INCLUDE_DIR}/glad/glad.c"
)


add_subdirectory("${PROJECT_INCLUDE_DIR}/glm")
add_subdirectory("${PROJECT_INCLUDE_DIR}/GLFW")
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw OpenGL::GL)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::boost Boost::headers)


target_include_directories(${PROJECT_NAME} PUBLIC
    "${PROJECT_SCRIPTS_DIR}" "${PROJECT_INCLUDE_DIR}" "${PROJECT_BINARY_DIR}")
                           