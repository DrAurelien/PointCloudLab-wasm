cmake_minimum_required(VERSION 3.10)

# List source files
file(GLOB
    SOURCES
    src/PointCloud.bindings.cpp
)

# List HTML app files
file(GLOB
    HTML
    index.html
)

# Declare build target
add_executable(PointCloudLab ${SOURCES})

# Include directories
target_include_directories(PointCloudLab PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Linker properties
set_target_properties(PointCloudLab
    PROPERTIES
    LINK_FLAGS "-s WASM=1 -s ALLOW_MEMORY_GROWTH -s NO_DISABLE_EXCEPTION_CATCHING -lembind --bind")

# Link the kernel library
target_link_libraries(PointCloudLab PUBLIC PointCloudLabKernel)

# Setup compiler options
target_compile_features(PointCloudLab PRIVATE cxx_std_20)
add_compile_options(
    PointCloudLab
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(
        PointCloudLab
        PRIVATE
            -O0
            -g
    )
else()
    add_compile_options(
        PointCloudLab
        PRIVATE
            -O3
    )
endif()

# Post build step : copy HTML source files to the build dir
add_custom_command(
    TARGET
    PointCloudLab
    POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E copy ${HTML} ${CMAKE_CURRENT_BINARY_DIR}
)

# Install : copy all the files required to run the app to a clean install dir.
install(
    FILES
        ${HTML}
        ${CMAKE_CURRENT_BINARY_DIR}/PointCloudLab.js
        ${CMAKE_CURRENT_BINARY_DIR}/PointCloudLab.wasm

    DESTINATION
        ${CMAKE_INSTALL_PREFIX}
)