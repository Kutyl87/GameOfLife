cmake_minimum_required(VERSION 3.16.0)
project(bgfx_map VERSION 0.1.0)
set(SOURCE_FILES main.cpp)
set(MODE Debug)
add_definitions(-D BX_CONFIG_DEBUG=1)
Include(FetchContent)
FetchContent_Declare(
    bx
    GIT_REPOSITORY https://github.com/bkaradzic/bx.git
    GIT_TAG master
)
FetchContent_GetProperties(bx)
if(NOT bx_POPULATED)
    message("Started cloning BX ...")
    FetchContent_Populate(bx)
    message("Finished cloning BX")
    execute_process(COMMAND git submodule update --init --recursive
    WORKING_DIRECTORY ${bx_SOURCE_DIR})
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${bx_SOURCE_DIR} ${CMAKE_BINARY_DIR}/_deps/bx)
endif()
FetchContent_Declare(
    bimg
    GIT_REPOSITORY https://github.com/bkaradzic/bimg.git
    GIT_TAG master
)
FetchContent_GetProperties(bimg)
if(NOT bimg_POPULATED)
    message("Started cloning BIMG ...")
    FetchContent_Populate(bimg)
    message("Finished cloning BIMG")
    execute_process(COMMAND git submodule update --init --recursive
    WORKING_DIRECTORY ${bimg_SOURCE_DIR})
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${bimg_SOURCE_DIR} ${CMAKE_BINARY_DIR}/_deps/bimg)
endif()
FetchContent_Declare(
    bgfx
    GIT_REPOSITORY https://github.com/bkaradzic/bgfx.git
    GIT_TAG master
)
FetchContent_GetProperties(bgfx)
if(NOT bgfx_POPULATED)
    message("Started cloning BGFX ...")
    FetchContent_Populate(bgfx)
    message("Finished cloning BGFX")
    execute_process(COMMAND git submodule update --init --recursive
    WORKING_DIRECTORY ${bgfx_SOURCE_DIR})
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${bgfx_SOURCE_DIR} ${CMAKE_BINARY_DIR}/_deps/bgfx)
endif()
add_executable(bgfx_map ${SOURCE_FILES})
set_target_properties(bgfx_map
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)
add_custom_target(make_bx
    COMMAND make ${PLATFORM} -C ${CMAKE_BINARY_DIR}/_deps/bx/
)
add_custom_target(make_bimg
    COMMAND make ${PLATFORM} -C ${CMAKE_BINARY_DIR}/_deps/bimg/
)
add_custom_target(make_bgfx
    COMMAND make ${PLATFORM} -C ${CMAKE_BINARY_DIR}/_deps/bgfx/
)
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
target_include_directories(bgfx_map PRIVATE 
                           ${CMAKE_BINARY_DIR}/_deps/bx/include
                           ${CMAKE_BINARY_DIR}/_deps/bgfx/include
                           ${CMAKE_BINARY_DIR}/_deps/bimg/include)
target_link_directories(bgfx_map PRIVATE
    ${CMAKE_BINARY_DIR}/_deps/bx/.build/${PLATFORM}/bin
    ${CMAKE_BINARY_DIR}/_deps/bgfx/.build/${PLATFORM}/bin
    ${CMAKE_BINARY_DIR}/_deps/bimg/.build/${PLATFORM}/bin)
target_link_libraries(bgfx_map 
                        OpenGL::GL
                        glfw
                        bx${MODE}
                        bgfx${MODE}
                        bimg${MODE}
                        )
add_dependencies(bgfx_map make_bx make_bimg make_bgfx)