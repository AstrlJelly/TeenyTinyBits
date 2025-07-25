cmake_minimum_required(VERSION 3.31)

project(TeenyTinyBits VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(PROJECT_SCRIPTS_DIR ${PROJECT_SOURCE_DIR}/scripts)
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

configure_file(${PROJECT_SCRIPTS_DIR}/constants.hpp.in ${PROJECT_BINARY_DIR}/constants.hpp)

# yes i know this isn't how you should do it. 😒
add_executable(${PROJECT_NAME} 
    # glad
    ${PROJECT_INCLUDE_DIR}/glad/glad.c

    # imgui
    ${PROJECT_INCLUDE_DIR}/ImGui/imgui_impl_opengl3.cpp
    ${PROJECT_INCLUDE_DIR}/ImGui/imgui.cpp
    ${PROJECT_INCLUDE_DIR}/ImGui/imgui_draw.cpp
    ${PROJECT_INCLUDE_DIR}/ImGui/imgui_tables.cpp
    ${PROJECT_INCLUDE_DIR}/ImGui/imgui_widgets.cpp

    # project
    ${PROJECT_SCRIPTS_DIR}/main.cpp
    ${PROJECT_SCRIPTS_DIR}/shader.cpp
    ${PROJECT_SCRIPTS_DIR}/game_window.cpp
    ${PROJECT_SCRIPTS_DIR}/input_manager.cpp)

add_subdirectory(${PROJECT_INCLUDE_DIR}/glm)
add_subdirectory(${PROJECT_INCLUDE_DIR}/GLFW)
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw OpenGL::GL)

# add_subdirectory(${PROJECT_INCLUDE_DIR}/ImGui)

target_include_directories(${PROJECT_NAME} PUBLIC
    "${PROJECT_SCRIPTS_DIR}"
    "${PROJECT_INCLUDE_DIR}"
    "${PROJECT_BINARY_DIR}")
                           