cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
project(GameOfLife)

add_executable(GameOfLife source/main.cpp
						  source/GmOb/tiles.cpp
						  source/GmOb/tiles.h
						  source/Renderer/Texture.cpp
                          source/Renderer/Texture.h
						  source/Renderer/ShaderClass.cpp
                          source/Renderer/ShaderClass.h
						  source/stb_image/stb_image.h)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(GameOfLife glfw)

add_subdirectory(external/glad)
target_link_libraries(GameOfLife glad)

set_target_properties(GameOfLife PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET GameOfLife POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:GameOfLife>/res)