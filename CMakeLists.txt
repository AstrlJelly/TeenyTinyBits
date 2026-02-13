cmake_minimum_required(VERSION 3.27)

if (NOT DEFINED CMAKE_CXX_COMPILER)
  set(CMAKE_C_COMPILER clang)
  set(CMAKE_CXX_COMPILER clang++)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(teeny VERSION 0.0.0)

set(PROJECT_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(PROJECT_SCRIPTS_DIR "${PROJECT_SOURCE_DIR}/src")
set(DEMO_DIR "${PROJECT_SOURCE_DIR}/demo")
set(DEMO_SCRIPTS_DIR "${DEMO_DIR}/src")

file(GLOB_RECURSE PROJECT_SCRIPTS "${PROJECT_SCRIPTS_DIR}/*.cpp")
file(GLOB_RECURSE PROJECT_HEADERS "${PROJECT_SCRIPTS_DIR}/*.hpp")

add_library(teeny STATIC ${PROJECT_SCRIPTS})

if(MSVC)
  target_compile_options(teeny PRIVATE /W2)
else()
  target_compile_options(teeny PRIVATE -Wall -Wextra)
endif()

target_sources(teeny
  PRIVATE ${PROJECT_SCRIPTS}

  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      src/
    FILES
      ${PROJECT_HEADERS}
)

target_include_directories(teeny PRIVATE
    "${PROJECT_SCRIPTS_DIR}" "${PROJECT_INCLUDE_DIR}" "${PROJECT_BINARY_DIR}")

# libraries
find_package(OpenGL REQUIRED)

add_subdirectory("${PROJECT_INCLUDE_DIR}/glm")
add_subdirectory("${PROJECT_INCLUDE_DIR}/GLFW")

set(IMGUI_DIR "${PROJECT_INCLUDE_DIR}/ImGui")
file(GLOB IMGUI_SCRIPTS
    "${IMGUI_DIR}/*.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)
add_library(ImGui STATIC ${IMGUI_SCRIPTS})
target_include_directories(ImGui PUBLIC "${PROJECT_INCLUDE_DIR}")
# target_link_libraries(ImGui PRIVATE glfw)

add_library(glad STATIC "${PROJECT_INCLUDE_DIR}/glad/glad.c")
target_include_directories(glad PUBLIC "${PROJECT_INCLUDE_DIR}")
# end libraries

target_link_libraries(teeny PUBLIC
    glfw OpenGL::GL ImGui glad)

file(GLOB_RECURSE DEMO_SCRIPTS "${DEMO_DIR}/*.cpp")
add_executable(teenydemo ${DEMO_SCRIPTS})
target_link_libraries(teenydemo PRIVATE teeny)
