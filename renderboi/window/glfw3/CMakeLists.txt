# Set up external dependencies
include( FetchContent )

FetchContent_Declare(
    glfw3
    DOWNLOAD_EXTRACT_TIMESTAMP ON
    URL https://github.com/glfw/glfw/archive/refs/tags/3.4.zip
    FIND_PACKAGE_ARGS 3.4
)

set( GLFW_BUILD_DOCS OFF )
set( GLFW_BUILD_EXAMPLES OFF )
set( GLFW_BUILD_TESTS OFF )

FetchContent_MakeAvailable( glfw3 )

####################################################
# Which policy to use for borderless windowed mode #
####################################################

# GLFW does not provide a way to get the desktop video mode of a monitor,
# which is needed in order to allow for "borderless windowed" mode.
#
# RenderBoi will try to do this in one of two ways:
# - Native mode policy: RenderBoi remembers the current video modes of all
# monitors when it first starts up, and uses those to go borderless windowed
# on a given monitor.
# - Max mode policy: RenderBoi assumes the desktop video mode of a monitor
# simply is the "largest" available video mode of that monitor, and will use
# it to go borderless windowed on that monitor.
#
# Unfortunately, neither of these methods guarantees that the resulting
# video mode actually is the desktop video mode of any given monitor.
#
# The default policy for going borderless windowed is the native mode 
# policy. Provide the following at configure time to change the policy:
# -DGLFW3_BORDERLESS_POLICY_MAX_MODE=ON
# The variables desclared hereafter will be used to configure the header
# file ./config/env_info.config.hpp.

# Default policy is native mode policy
set( GLFW3_BORDERLESS_POLICY_NATIVE_MODE_VAL 1 PARENT_SCOPE )
set( GLFW3_BORDERLESS_POLICY_MAX_MODE_VAL    0 PARENT_SCOPE )

# If told otherwise...
if( GLFW3_BORDERLESS_POLICY_MAX_MODE AND NOT GLFW3_BORDERLESS_POLICY_NATIVE_MODE )
    # Set options accordingly
    set( GLFW3_BORDERLESS_POLICY_NATIVE_MODE     OFF PARENT_SCOPE )
    set( GLFW3_BORDERLESS_POLICY_NATIVE_MODE_VAL 0   PARENT_SCOPE )
    set( GLFW3_BORDERLESS_POLICY_MAX_MODE_VAL    1   PARENT_SCOPE )
endif( )

add_library( renderboi_glfw3_adapter
    glfw3_adapter.cpp
    glfw3_adapter.hpp
    glfw3_gamepad_manager.cpp
    glfw3_gamepad_manager.hpp
    glfw3_monitor.cpp
    glfw3_monitor.hpp
    glfw3_utilities.cpp
    glfw3_utilities.hpp
    glfw3_window_factory.cpp
    glfw3_window_factory.hpp
    glfw3_window.cpp
    glfw3_window.hpp
)

target_include_directories( renderboi_glfw3_adapter PUBLIC
    ${glfw3_SOURCE_DIR}/include
    ${RENDERBOI_MAIN_INCLUDE_PATH}
)

target_link_libraries( renderboi_glfw3_adapter PRIVATE
    glfw
    glad
    cpptools::cpptools_static
    renderboi_utilities
)
