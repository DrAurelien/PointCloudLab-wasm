if(EMSCRIPTEN)
    message(STATUS "Retrieving OpenGLES3 for WebAssembly")
    set(OPENGLES3_INCLUDE_DIRS "")  # Emscripten provides GLES3 headers automatically
    set(OPENGLES3_LIBRARIES "")     # Emscripten links GLES3 automatically
    set(OPENGLES3_FOUND TRUE)
else()
    if(LINUX OR ANDROID)
        find_path(OPENGLES3_INCLUDE_DIRS GLES3/gl3.h
            PATHS
                /usr/include
                /usr/local/include
                /opt/include
                /opt/local/include
        )

        find_library(OPENGLES3_LIBRARIES
            NAMES GLESv3
            PATHS
                /usr/lib
                /usr/local/lib
                /opt/lib
                /opt/local/lib
        )

        include(FindPackageHandleStandardArgs)
        find_package_handle_standard_args(OpenGLES3 DEFAULT_MSG
            OPENGLES3_LIBRARIES
            OPENGLES3_INCLUDE_DIRS
        )

        mark_as_advanced(OPENGLES3_INCLUDE_DIRS OPENGLES3_LIBRARIES)
    endif()
endif()
