set(Boost_USE_STATIC_LIBS OFF)
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})




file(GLOB TEST_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(tests ${TEST_FILES})

target_compile_features(tests PRIVATE cxx_std_17)
target_link_libraries(tests PRIVATE
    foliage_generator_lib
    ${Boost_LIBRARIES}
    loguru::loguru
)

enable_testing()

add_test(NAME tests_run COMMAND tests)