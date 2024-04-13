cmake_minimum_required(VERSION 3.16.0)
project(bgfx_map VERSION 0.1.0)
add_executable(bgfx_map main.cpp)
Include(FetchContent)
FetchContent_Declare(
    bgfx
    GIT_REPOSITORY 
)