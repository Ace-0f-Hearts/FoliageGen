cmake_minimum_required(VERSION 3.31)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

# Add source files
file(GLOB SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)
message("${SOURCE_FILES}")

# Add header files
file(GLOB HEADER_FILES
        ${CMAKE_SOURCE_DIR}/*.h
        ${CMAKE_SOURCE_DIR}/*.hpp)



add_library(foliage_generator_lib ${HEADER_FILES} ${SOURCE_FILES})

add_subdirectory(botanics_math)
add_subdirectory(ocad)
add_subdirectory(utility)
add_subdirectory(spatial)
add_subdirectory(orienteering)
add_subdirectory(foliage)

message("${CMAKE_SOURCE_DIR}")

target_link_libraries(foliage_generator_lib
        PUBLIC
        spatial
        utility
        ocad
        botanics_math
        orienteering
        foliage
        PRIVATE
        jsoncpp_lib
        loguru::loguru
)

source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header files"
        FILES ${HEADER_LIST}
)