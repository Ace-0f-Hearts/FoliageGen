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

target_include_directories(foliage
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

FETCHCONTENT_DECLARE(cimg
        GIT_REPOSITORY https://github.com/GreycLab/CImg.git
        GIT_TAG "master"
)

FETCHCONTENT_MAKEAVAILABLE(cimg)

target_compile_features(foliage PUBLIC cxx_std_11)
target_compile_definitions(foliage PRIVATE
        cimg_use_png
        cimg_use_zlib
        cimg_use_jpeg
        cimg_display=0
)

target_link_libraries(foliage PRIVATE
        cimg
        loguru::loguru
        alglib
        nanoflann::nanoflann
)
source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST})
