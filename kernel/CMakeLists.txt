cmake_minimum_required(VERSION 3.10)

# List kernel source files
file(GLOB
    SOURCES
    src/model/PointCloud.cpp
    src/io/CSVFiles.cpp
)

# Declare build target
add_library(PointCloudLabKernel ${HEADERS} ${SOURCES})

# Include directories
target_include_directories(PointCloudLabKernel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Setup compiler options
target_compile_features(PointCloudLabKernel PRIVATE cxx_std_20)
add_compile_options(
    PointCloudLabKernel
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(
        PointCloudLabKernel
        PRIVATE
            -O0
            -g
    )
else()
    add_compile_options(
        PointCloudLabKernel
        PRIVATE
            -O3
    )
endif()