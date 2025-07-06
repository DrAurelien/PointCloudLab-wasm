cmake_minimum_required(VERSION 3.27)

# List kernel source files
file(GLOB
    MODEL_SOURCES
    src/Model/PointCloud.cpp
)

file(
    GLOB
    IO_SOURCES
    src/IO/CSVFiles.cpp
)

file(GLOB
    RENDERING_SOURCES
    src/Rendering/RenderablePointCloud.cpp
)

set(SOURCES "")
list(APPEND SOURCES
    ${MODEL_SOURCES}
    ${IO_SOURCES}
)
if(BUILD_RENDERING)
    list(APPEND SOURCES ${RENDERING_SOURCES})
endif()

# Find GLES3
find_package(OpenGLES3 REQUIRED)

# Declare build target
add_library(PointCloudLabKernel ${HEADERS} ${SOURCES})

# Include directories
target_include_directories(PointCloudLabKernel 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OPENGLES3_INCLUDE_DIRS}
)

target_link_libraries(PointCloudLabKernel 
    PUBLIC 
        ${OPENGLES3_LIBRARIES}
)

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