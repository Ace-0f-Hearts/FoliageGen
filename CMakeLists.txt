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

find_package(loguru CONFIG REQUIRED)
message(STATUS "loguru included")

find_package(SDL3 REQUIRED)
message(STATUS "SDL3 included")

find_package(jsoncpp REQUIRED)
message(STATUS "JSONCpp included")

add_subdirectory(src)
add_subdirectory(apps)
add_subdirectory(tests)

include_directories(src)

## Add source files
#file(GLOB_RECURSE SOURCE_FILES
#        ${CMAKE_SOURCE_DIR}/src/*.c
#        ${CMAKE_SOURCE_DIR}/src/*.cpp
#
#)
#
## Add header files
#file(GLOB_RECURSE HEADER_FILES
#        ${CMAKE_SOURCE_DIR}/src/*.h
#        ${CMAKE_SOURCE_DIR}/src/*.hpp)
#
##${HEADER_FILES} ${SOURCE_FILES}
#
#add_library(Generator main.cpp ${HEADER_FILES} ${SOURCE_FILES})
#add_executable(App main.cpp)
#
#find_package(loguru CONFIG REQUIRED)
#message(STATUS "loguru included at${loguru_INCLUDE_DIR}")
#
#find_package(SDL3 REQUIRED)
#message(STATUS "SDL3 included at ${SDL3_INCLUDE_DIR}")
#
#find_package(jsoncpp REQUIRED)
#message(STATUS "JSONCpp included at ${jsoncpp_INCLUDE_DIR}")
#
#target_link_libraries(Generator PRIVATE
#    jsoncpp
#    SDL3::SDL3
#    loguru::loguru
#)
#
#target_link_libraries(App PRIVATE
#
#    Generator
#)
#
#add_subdirectory(tests)