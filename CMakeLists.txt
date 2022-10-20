cmake_minimum_required(VERSION 3.10)

project(SnakeClone)

option(SDL_STATIC "" OFF)

include(FetchContent)
FetchContent_Declare(SDL2 GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git" GIT_TAG "release-2.24.1")
FetchContent_MakeAvailable(SDL2)

add_executable(SnakeClone game.c)

target_link_libraries(SnakeClone SDL2)

target_compile_options(SnakeClone PRIVATE -Wall -Wextra -pedantic -Werror)

set_target_properties(SnakeClone PROPERTIES C_STANDARD 11)
