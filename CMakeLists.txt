cmake_minimum_required(VERSION 3.31)
project(generator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

# Add source files
file(GLOB_RECURSE SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/*.c
        ${CMAKE_SOURCE_DIR}/src/*.cpp

)

# Add header files
file(GLOB_RECURSE HEADER_FILES
        ${CMAKE_SOURCE_DIR}/src/*.h
        ${CMAKE_SOURCE_DIR}/src/*.hpp)


add_executable(generator main.cpp ${HEADER_FILES} ${SOURCE_FILES})

find_package(loguru CONFIG REQUIRED)
message(STATUS "loguru included at${loguru_INCLUDE_DIR}")

find_package(SDL3 REQUIRED)
message(STATUS "SDL3 included at ${SDL3_INCLUDE_DIR}")

find_package(jsoncpp REQUIRED)
message(STATUS "JSONCpp included at ${jsoncpp_INCLUDE_DIR}")

target_link_libraries(${PROJECT_NAME} PRIVATE
    jsoncpp
    SDL3::SDL3
    loguru::loguru
)