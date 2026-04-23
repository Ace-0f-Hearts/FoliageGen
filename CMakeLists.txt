cmake_minimum_required(VERSION 3.31)
project(
    ProceduralFoliageGenerator
    VERSION 0.1
    DESCRIPTION "A procedural foliage generator based on orienteering maps"
    LANGUAGES CXX
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED  True)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_DEBUG "-O3")


if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_BUILD_TYPE Debug)

    set(CMAKE_CXX_EXTENSIONS OFF)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    include(CTest)

    find_package(Doxygen)
    if (Doxygen_FOUND)
        add_subdirectory(docs)
    else()
        message(STATUS "Doxygen not found, not building docs")
    endif()

endif ()

set(BOOST_INCLUDE_LIBRARIES test heap)
set(BOOST_ENABLE_CMAKE ON)
set(LOGURU_WITH_STREAMS TRUE)
find_package(glm REQUIRED)
find_package(Boost COMPONENTS heap unit_test_framework REQUIRED)
find_package(loguru REQUIRED)
find_package(nanoflann REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(CImg REQUIRED)
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
find_package(CGAL REQUIRED COMPONENTS Core)



message("FOUND CONFIG: ${jsoncpp_CONFIG}")
message("INCLUDE DIRS: ${jsoncpp_INCLUDE_DIRS}")
message("LIBRARIES: ${jsoncpp_LIBRARIES}")

set(Boost_USE_STATIC_LIBS OFF)
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

get_property(all_targets DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)

add_subdirectory(src)
add_subdirectory(apps)
add_subdirectory(tests)


