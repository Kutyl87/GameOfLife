cmake_minimum_required(VERSION 3.16.0)
project(bgfx_map VERSION 0.1.0)
set(SOURCE_FILES main.cpp)
Include(FetchContent)
FetchContent_Declare(
    bx
    GIT_REPOSITORY https://github.com/bkaradzic/bx.git
    GIT_TAG master
)
FetchContent_GetProperties(bx)
if(NOT bx_POPULATED)
    FetchContent_Populate(bx)
    execute_process(COMMAND git submodule update --init --recursive
    WORKING_DIRECTORY ${bx_SOURCE_DIR})
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${bx_SOURCE_DIR} ${CMAKE_BINARY_DIR}/_deps/bx)
    execute_process(COMMAND make ${PLATFORM} -C ${CMAKE_BINARY_DIR}/_deps/bx/)
endif()
FetchContent_Declare(
    bimg
    GIT_REPOSITORY https://github.com/bkaradzic/bimg.git
    GIT_TAG master
)
FetchContent_GetProperties(bimg)
if(NOT bimg_POPULATED)
    FetchContent_Populate(bimg)
    execute_process(COMMAND git submodule update --init --recursive
    WORKING_DIRECTORY ${bimg_SOURCE_DIR})
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${bimg_SOURCE_DIR} ${CMAKE_BINARY_DIR}/_deps/bimg)
    execute_process(COMMAND make ${PLATFORM} -C ${CMAKE_BINARY_DIR}/_deps/bimg/)
endif()
FetchContent_Declare(
    bgfx
    GIT_REPOSITORY https://github.com/bkaradzic/bgfx.git
    GIT_TAG master
)
FetchContent_Declare(
    bnet
    GIT_REPOSITORY https://github.com/bkaradzic/bnet.git
    GIT_TAG master
)
FetchContent_GetProperties(bnet)
if(NOT bnet_POPULATED)
    FetchContent_Populate(bnet)
    execute_process(COMMAND git submodule update --init --recursive
    WORKING_DIRECTORY ${bnet_SOURCE_DIR})
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${bimg_SOURCE_DIR} ${CMAKE_BINARY_DIR}/_deps/bnet)
    execute_process(COMMAND make ${PLATFORM} -C ${CMAKE_BINARY_DIR}/_deps/bnet/)
endif()
add_executable(bgfx_map ${SOURCE_FILES})
FetchContent_GetProperties(bgfx)
if(NOT bgfx_POPULATED)
    FetchContent_Populate(bgfx)
    execute_process(COMMAND git submodule update --init --recursive
    WORKING_DIRECTORY ${bgfx_SOURCE_DIR})
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${bgfx_SOURCE_DIR} ${CMAKE_BINARY_DIR}/_deps/bgfx)
    execute_process(COMMAND make ${PLATFORM} -C ${CMAKE_BINARY_DIR}/_deps/bgfx/)
endif()