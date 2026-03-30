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

FetchContent_Declare(zlib
        GIT_REPOSITORY https://github.com/madler/zlib.git
        GIT_TAG "v1.3.1"
)
FetchContent_MakeAvailable(zlib)

FetchContent_Declare(libpng
        GIT_REPOSITORY https://github.com/pnggroup/libpng.git
        GIT_TAG "v1.6.43"
)
FetchContent_MakeAvailable(libpng)

include(ExternalProject)
set(LIBJPEG_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/libjpeg-turbo-install)
message(STATUS "Current binary dir: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "LIBJPEG install dir: ${LIBJPEG_INSTALL_DIR}")
ExternalProject_Add(
        libjpeg_turbo
        GIT_REPOSITORY https://github.com/libjpeg-turbo/libjpeg-turbo.git
        GIT_TAG        3.0.3
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${LIBJPEG_INSTALL_DIR}
        -DENABLE_SHARED=OFF
        -DENABLE_STATIC=ON
        BUILD_BYPRODUCTS
        ${LIBJPEG_INSTALL_DIR}/lib64/libturbojpeg.a
)

# Create the directory so CMake doesn't complain at configure time
file(MAKE_DIRECTORY ${LIBJPEG_INSTALL_DIR}/include)

# Create imported target
add_library(turbojpeg-static STATIC IMPORTED)
set_target_properties(turbojpeg-static PROPERTIES
        IMPORTED_LOCATION ${LIBJPEG_INSTALL_DIR}/lib64/libturbojpeg.a
        INTERFACE_INCLUDE_DIRECTORIES ${LIBJPEG_INSTALL_DIR}/include
)
add_dependencies(turbojpeg-static libjpeg_turbo)
FETCHCONTENT_DECLARE(cimg
        GIT_REPOSITORY https://github.com/GreycLab/CImg.git
        GIT_TAG "master"
)

FETCHCONTENT_MAKEAVAILABLE(cimg)


file(GLOB ALGLIB_SOURCES
        "${CMAKE_SOURCE_DIR}/external/alglib-cpp/src/*.cpp"
)
file(GLOB ALGLIB_HEADER
        "${CMAKE_SOURCE_DIR}/external/alglib-cpp/src/*.h"
)

add_library(alglib STATIC ${ALGLIB_SOURCES} ${ALGLIB_HEADER})
target_include_directories(alglib PRIVATE "${CMAKE_SOURCE_DIR}/external/alglib-cpp/src")


add_library(foliage ${HEADER_LIST} ${SOURCE_LIST})

target_include_directories(foliage
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${LIBJPEG_INSTALL_DIR}/include
        ${libpng_SOURCE_DIR}
        ${libpng_BINARY_DIR}
        ${cimg_SOURCE_DIR}
)
add_dependencies(foliage libjpeg_turbo)

target_compile_features(foliage PUBLIC cxx_std_11)
target_compile_definitions(foliage PRIVATE
        cimg_use_png
        cimg_use_zlib
        cimg_use_jpeg
        cimg_display=0
)

target_link_libraries(foliage PRIVATE
        loguru::loguru
        nanoflann::nanoflann
        jsoncpp_lib
        alglib
        zlibstatic
        png_static
        turbojpeg-static

)
source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST})
