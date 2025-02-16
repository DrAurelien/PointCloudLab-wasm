# Minimum CMake version
cmake_minimum_required(VERSION 3.15)

# Enable testing
enable_testing()

# Add source files for the unit test executable
# Download and include GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add source files for the unit test executable
file(GLOB
    TEST_SOURCES
    CSVRead-test.cpp
)

# Create the test executable
add_executable(PointCloudLabTests ${TEST_SOURCES})

target_compile_features(PointCloudLabTests PRIVATE cxx_std_20)

# Link GoogleTest to the test executable
target_link_libraries(PointCloudLabTests
    PRIVATE
    gtest
    gtest_main
    PointCloudLabKernel
)

# Add the test executable to CTest
add_test(NAME PointCloudLabTests COMMAND PointCloudLabTests)