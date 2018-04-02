cmake_minimum_required(VERSION 3.8)
project(SnakeClone)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -Wno-unused-parameter")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SDL2 REQUIRED)

add_executable(${PROJECT_NAME} game.c)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})