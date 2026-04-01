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

include(FetchContent)

FETCHCONTENT_DECLARE(glm
        GIT_REPOSITORY	https://github.com/g-truc/glm.git
        GIT_TAG "master"
)

FETCHCONTENT_MAKEAVAILABLE(glm)


set(LOGURU_WITH_STREAMS TRUE)
FetchContent_Declare(
        loguru
        GIT_REPOSITORY https://github.com/emilk/loguru
        GIT_TAG "master"
#        FIND_PACKAGE_ARGS NAMES loguru
)

FetchContent_Declare(
        jsoncpp
        GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp
        GIT_TAG 1.9.7
#        FIND_PACKAGE_ARGS NAMES jsoncpp
)




set(BOOST_INCLUDE_LIBRARIES test heap)
set(BOOST_ENABLE_CMAKE ON)

set(Boost_USE_STATIC_LIBS OFF)
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)
FETCHCONTENT_DECLARE(
        Boost
        GIT_REPOSITORY https://github.com/boostorg/boost
        GIT_TAG boost-1.90.0
        GIT_SHALLOW TRUE
#        FIND_PACKAGE_ARGS COMPONENTS unit_test_framework REQUIRED
)

FetchContent_MakeAvailable(loguru jsoncpp Boost)

get_property(all_targets DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)

add_subdirectory(src)
add_subdirectory(apps)
add_subdirectory(tests)


