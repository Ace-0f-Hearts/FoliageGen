cmake_minimum_required(VERSION 3.31)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

file(GLOB SOURCE_LIST
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

file(GLOB HEADER_LIST CONFIGURE_DEPENDS
        ${ProceduralFoliageGenerator_SOURCE_DIR}/include/orienteering/*.h
        ${ProceduralFoliageGenerator_SOURCE_DIR}/include/orienteering/*.hpp
)

add_library(orienteering ${HEADER_LIST} ${SOURCE_LIST})



target_include_directories(orienteering PUBLIC ${CMAKE_SOURCE_DIR}/include ${glm_SOURCE_DIR})
target_compile_features(orienteering PUBLIC cxx_std_11)
target_link_libraries(orienteering PUBLIC
        spatial
        glm::glm
        PRIVATE
        loguru::loguru
)


source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST})
