################################################################################
#                                                                              #
#                             CMake configuration                              #
#                                                                              #
################################################################################

cmake_minimum_required(VERSION 3.11)

################################################################################
#                                                                              #
#                                   Toggles                                    #
#                                                                              #
################################################################################

option( RENDERBOI_SKIP_TESTS "Whether or not to skip tests" OFF )

if( WIN32 AND BUILD_SHARED_LIBS )
    set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )
endif( )

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

################################################################################
#                                                                              #
#                             Project definition                               #
#                                                                              #
################################################################################

# Project general info
set(PROJECT_VERSION "1.0")
set(PROJECT_NAME "RenderBoi")

project(
    ${PROJECT_NAME}
    VERSION ${PROJECT_VERSION}
    DESCRIPTION "A rendering engine with a modest feature set"
    LANGUAGES CXX
)

################################################################################
#                                                                              #
#                            Project-wide toggles                              #
#                                                                              #
################################################################################

option(WINDOW_BACKEND_GLFW3 "Use GLFW3 as the window backend" ON)
option(
    GLFW3_BORDERLESS_POLICY_NATIVE
    "Have GLFW3 detect borderless fullscreen parameters upon starting up
    (default, overrides GLFW3_BORDERLESS_POLICY_MAX_MODE)"
    ON
)
option(
    GLFW3_BORDERLESS_POLICY_MAX_MODE
    "Have GLFW3 use the largest available video mode for borderless fullscreen"
    OFF
)

################################################################################
#                                                                              #
#                                Dependencies                                  #
#                                                                              #
################################################################################

include( FetchContent )

# cpptools
FetchContent_Declare(
    cpptools
    DOWNLOAD_EXTRACT_TIMESTAMP ON
    URL https://github.com/deqyra/CppTools/archive/refs/tags/v0.1.zip
)

set( CPPTOOLS_SKIP_TESTS ON )

# stb
FetchContent_Declare(
    stb
    DOWNLOAD_EXTRACT_TIMESTAMP ON
	GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG ae721c50eaf761660b4f90cc590453cdb0c2acd0
)

# glm
FetchContent_Declare(
    glm
    DOWNLOAD_EXTRACT_TIMESTAMP ON
    URL https://github.com/g-truc/glm/archive/refs/tags/1.0.1.zip
)

enable_language( C )
set( GLM_ENABLE_CXX_20 ON )

FetchContent_MakeAvailable( cpptools stb glm )
    
# glad
add_subdirectory( external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad )

# std_image
add_subdirectory( external/stb_image ${CMAKE_CURRENT_BINARY_DIR}/stb_image )

################################################################################
#                                                                              #
#                         Build RenderBoi executable                           #
#                                                                              #
################################################################################

add_subdirectory( renderboi ${CMAKE_CURRENT_BINARY_DIR}/renderboi )

################################################################################
#                                                                              #
#                                 Unit tests                                   #
#                                                                              #
################################################################################

# Build and run tests
if( NOT RENDERBOI_SKIP_TESTS )
    add_subdirectory( tests )
endif(  )
