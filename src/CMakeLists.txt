cmake_minimum_required(VERSION 3.31)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

# Add source files
file(GLOB SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/orienteering/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/orienteering/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/foliage/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/foliage/*.cpp

)
message("${SOURCE_FILES}")

# Add header files
file(GLOB_RECURSE HEADER_FILES
        ${CMAKE_SOURCE_DIR}/*.h
        ${CMAKE_SOURCE_DIR}/*.hpp)

add_library(foliage_generator_lib ${HEADER_FILES} ${SOURCE_FILES})

add_subdirectory(botanics_math)
add_subdirectory(ocad)
add_subdirectory(utility)
add_subdirectory(spatial)
add_subdirectory(foliage)
add_subdirectory(orienteering)

message("${CMAKE_SOURCE_DIR}")

target_link_libraries(foliage_generator_lib
        PUBLIC #TODO: This is not nice, need to figure out a better way
        spatial
        utility
        ocad
        botanics_math
        orienteering
        foliage
        PRIVATE
        jsoncpp
        loguru::loguru
)

source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header files"
        FILES ${HEADER_LIST}
)