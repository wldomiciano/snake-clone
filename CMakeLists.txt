cmake_minimum_required(VERSION 3.10)

project(SnakeClone)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})

add_executable(SnakeClone game.c)

target_link_libraries(SnakeClone ${SDL2_LIBRARY})
target_compile_options(SnakeClone PRIVATE -Wall -Wextra -pedantic -Wno-unused-parameter)
set_target_properties(SnakeClone PROPERTIES C_STANDARD 11)