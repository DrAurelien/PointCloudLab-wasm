cmake_minimum_required(VERSION 3.10)
project(PointCloudLab)


file(GLOB_RECURSE SRC src/*.cpp)
add_executable(${CMAKE_PROJECT_NAME} ${SRC})

set_target_properties(${CMAKE_PROJECT_NAME}
    PROPERTIES
    LINK_FLAGS "-s WASM=1 -lembind --bind")

install(TARGETS ${CMAKE_PROJECT_NAME})
install(FILES ${CMAKE_PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})