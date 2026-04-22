cmake_minimum_required(VERSION 3.31)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

# Add source files
file(GLOB SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# Add header files
file(GLOB HEADER_FILES
        ${CMAKE_SOURCE_DIR}/*.h
        ${CMAKE_SOURCE_DIR}/*.hpp)



add_library(foliage_generator_lib INTERFACE)

add_subdirectory(botanics_math)
add_subdirectory(ocad)
add_subdirectory(utility)
add_subdirectory(spatial)
add_subdirectory(orienteering)
add_subdirectory(foliage)


target_link_libraries(foliage_generator_lib
        INTERFACE
        spatial
        utility
        ocad
        botanics_math
        orienteering
        foliage
        jsoncpp_lib
        loguru::loguru
)

source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header files"
        FILES ${HEADER_LIST}
)