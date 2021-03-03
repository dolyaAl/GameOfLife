cmake_minimum_required(VERSION 3.19 FATAL_ERROR)


project(GameOfLife)

add_executable(GameOfLife source/main.cpp)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(GameOfLife glfw)

add_subdirectory(external/glad)
target_link_libraries(GameOfLife glad)
