add_executable(App main.cpp app.cpp app.h)

target_compile_features(App PRIVATE cxx_std_17)
target_link_libraries(App PUBLIC
        foliage_generator_lib
        loguru::loguru
        JsonCpp::JsonCpp
)

target_compile_options(App PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)