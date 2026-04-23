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


file(GLOB ALGLIB_SOURCES
        "${CMAKE_SOURCE_DIR}/external/alglib-cpp/src/*.cpp"
)
file(GLOB ALGLIB_HEADER
        "${CMAKE_SOURCE_DIR}/external/alglib-cpp/src/*.h"
)

add_library(alglib STATIC ${ALGLIB_SOURCES} ${ALGLIB_HEADER})
target_include_directories(alglib PUBLIC "${CMAKE_SOURCE_DIR}/external/alglib-cpp/src")


add_library(foliage STATIC ${HEADER_LIST} ${SOURCE_LIST})

target_include_directories(foliage
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${LIBJPEG_INSTALL_DIR}/include
        ${libpng_SOURCE_DIR}
        ${libpng_BINARY_DIR}
        ${cimg_SOURCE_DIR}
)

target_compile_features(foliage PUBLIC cxx_std_11)
target_compile_definitions(foliage PRIVATE
        cimg_use_png
        cimg_use_zlib
        cimg_use_jpeg
        cimg_display=0
)

target_link_libraries(foliage
        PUBLIC
        nanoflann::nanoflann
        botanics_math
        ocad
        orienteering
        spatial
        PRIVATE
        alglib
        loguru::loguru
        JsonCpp::JsonCpp
        PNG::PNG
        JPEG::JPEG
        glm::glm
        Boost::heap
        CGAL::CGAL

)
source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST})
