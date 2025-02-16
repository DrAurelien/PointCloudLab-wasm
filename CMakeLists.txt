cmake_minimum_required(VERSION 3.10)
project(PointCloudLab)

# Include Kernel sub directoy
add_subdirectory(kernel)

# List HTML app files
file(GLOB
    HTML
    index.html
)

if(EMSCRIPTEN)
    message (STATUS "Building for WebAssembly")
    add_subdirectory(bindings)
endif()

# Unit tests : include the tests directory if the UNIT_TEST variable is set to ON
if(UNIT_TESTS)
    message (STATUS "Building for Unit tests")
    add_subdirectory(tests)
endif()