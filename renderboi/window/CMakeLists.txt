# Detect EGL platform
set( EGL_DETECTED 0 )
if( UNIX )
    if (DEFINED ENV{WAYLAND_DISPLAY} )
        set( EGL_DETECTED 1 )
        message( STATUS "Wayland detected, EGL compatibility enabled." )
    endif( )
endif( )

# Config variables used in ./config/env_info.config.hpp
set( WINDOW_BACKEND "" )
set( WINDOW_BACKEND_ENUM "Unknown" )
set( USE_GLFW3 0 )

# Window backend selection
if( WINDOW_BACKEND_GLFW3 )
    set( WINDOW_BACKEND_ENUM "GLFW3" )
    set( USE_GLFW3 1 )
    add_subdirectory( glfw3 ${CMAKE_CURRENT_BINARY_DIR}/glfw3 )
    set( WINDOW_BACKEND renderboi_glfw3_adapter )
else( )
    message(FATAL_ERROR "No window backend was selected!")
endif( )

# Setup flags accordingly
configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/config/env_info.config.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/env_info.hpp" )

# Build window lib
add_library( renderboi_window
    enums.cpp
    enums.hpp
    env_info.hpp
    gl_window.cpp
    gl_window.hpp
    input_processor.cpp
    input_processor.hpp
    monitor.cpp
    monitor.hpp
    window_backend.hpp
    window_creation_parameters.hpp
    window_factory.hpp
    event/gl_context_event_manager.cpp
    event/gl_context_event_manager.hpp
    event/gl_context_event.cpp
    event/gl_context_event.hpp
    gamepad/gamepad_input_processor.cpp
    gamepad/gamepad_input_processor.hpp
    gamepad/gamepad_manager.cpp
    gamepad/gamepad_manager.hpp
    gamepad/gamepad_state.cpp
    gamepad/gamepad_state.hpp
    gamepad/gamepad.cpp
    gamepad/gamepad.hpp
)


set( THREADING_LIB "" )
if( UNIX )
    set( THREADING_LIB pthread )
endif( )

target_include_directories( renderboi_window PUBLIC ${RENDERBOI_MAIN_INCLUDE_PATH} )
target_link_libraries( renderboi_window PUBLIC
    ${CMAKE_DL_LIBS}
    ${WINDOW_BACKEND}
    ${THREADING_LIB}
    renderboi_utilities
)
