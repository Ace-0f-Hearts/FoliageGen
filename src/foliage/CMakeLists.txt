cmake_minimum_required(VERSION 3.31)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

file(GLOB SOURCE_LIST
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

file(GLOB HEADER_LIST CONFIGURE_DEPENDS
        ${ProceduralFoliageGenerator_SOURCE_DIR}/include/foliage/*.h
        ${ProceduralFoliageGenerator_SOURCE_DIR}/include/foliage/*.hpp
)

add_library(foliage ${HEADER_LIST} ${SOURCE_LIST})

target_include_directories(foliage PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_features(foliage PUBLIC cxx_std_11)
target_link_libraries(foliage PRIVATE
        loguru::loguru

)
source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST})
