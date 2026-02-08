cmake_minimum_required(VERSION 3.31)

# set(CMAKE_C_COMPILER clang)
# set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(teeny VERSION 0.0.0)

set(PROJECT_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(PROJECT_SCRIPTS_DIR "${PROJECT_SOURCE_DIR}/src")

file(GLOB_RECURSE PROJECT_SCRIPTS "${PROJECT_SCRIPTS_DIR}/*")


add_library(${PROJECT_NAME} STATIC ${PROJECT_SCRIPTS})

target_include_directories(${PROJECT_NAME} PUBLIC
    "${PROJECT_SCRIPTS_DIR}" "${PROJECT_INCLUDE_DIR}" "${PROJECT_BINARY_DIR}")

add_subdirectory("${PROJECT_INCLUDE_DIR}/glm")
add_subdirectory("${PROJECT_INCLUDE_DIR}/GLFW")
find_package(OpenGL REQUIRED)

set(IMGUI_DIR "${PROJECT_INCLUDE_DIR}/ImGui")
file(GLOB IMGUI_SCRIPTS
    "${IMGUI_DIR}/*.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)
add_library(ImGui STATIC ${IMGUI_SCRIPTS})
target_include_directories(ImGui PUBLIC "${PROJECT_INCLUDE_DIR}")

add_library(glad STATIC "${PROJECT_INCLUDE_DIR}/glad/glad.c")
target_include_directories(glad PUBLIC "${PROJECT_INCLUDE_DIR}")

target_link_libraries(${PROJECT_NAME} PUBLIC
    glfw OpenGL::GL ImGui glad)

add_executable(teenydemo "${PROJECT_SCRIPTS_DIR}/main.cpp")
target_link_libraries(teenydemo ${PROJECT_NAME})