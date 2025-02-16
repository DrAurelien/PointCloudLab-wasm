cmake_minimum_required(VERSION 3.10)
project(PointCloudLab)

# List source files
file(GLOB
    SOURCES

    # Kernel
    src/model/PointCloud.cpp
    
    # Bindings
    bindings/model/PointCloud.bindings.cpp
)

# List header files
file(GLOB
    HEADERS
    inlude/model/Vector.hpp
    inlude/model/PointCloud.hpp
)

# List HTML app files
file(GLOB
    HTML
    index.html
)

# Declare build target
include_directories(${CMAKE_PROJECT_NAME} include)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})
set_target_properties(${CMAKE_PROJECT_NAME}
    PROPERTIES
    LINK_FLAGS "-s WASM=1 -lembind --bind")

# Post build step : copy HTML source files to the build dir
add_custom_command(
    TARGET
        ${CMAKE_PROJECT_NAME}
    POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E copy ${HTML} ${CMAKE_BINARY_DIR}
)

# Install : copy all the files required to run the app to a clean install dir.
install(
    FILES
        ${HTML}
        ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.js
        ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.wasm

    DESTINATION
        ${CMAKE_INSTALL_PREFIX}
)