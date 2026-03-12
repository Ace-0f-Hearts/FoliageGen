include_directories(${Boost_INCLUDE_DIRS})

file(GLOB TEST_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(tests ${TEST_FILES})

add_custom_command(TARGET tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        ${CMAKE_CURRENT_BINARY_DIR}/assets
        COMMENT "Copy assets directory to testing directory")

target_compile_features(tests PRIVATE cxx_std_17)
target_link_libraries(tests PRIVATE
    foliage_generator_lib
    ${Boost_LIBRARIES}
    Boost::unit_test_framework
    loguru::loguru
)


enable_testing()

add_test(NAME tests_run COMMAND tests)