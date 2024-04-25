################################################################################
#                                                                              #
#                         Build RenderBoi executable                           #
#                                                                              #
################################################################################

set( PROJECT_MAINTAINER_NAME "François Brachais" )
set( PROJECT_MAINTAINER_EMAIL "francois.brachais@free.fr" )

# GL loader information
set( GL_CONTEXT_VERSION_MAJOR "4" )
set( GL_CONTEXT_VERSION_MINOR "2" )

# Copyleft notice generation
string( TIMESTAMP CURRENT_YEAR "%Y" )
set( DATE_SPAN "2019-${CURRENT_YEAR}" )

set( PROJECT_COPYRIGHT "${DATE_SPAN} © ${PROJECT_MAINTAINER_NAME} <${PROJECT_MAINTAINER_EMAIL}>" )

# File configuration
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config/project_env.config.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/project_env.hpp
)

# RenderBoi modules
add_subdirectory( utilities   ${CMAKE_CURRENT_BINARY_DIR}/renderboi.utilities )
add_subdirectory( core        ${CMAKE_CURRENT_BINARY_DIR}/renderboi.core )
add_subdirectory( window      ${CMAKE_CURRENT_BINARY_DIR}/renderboi.window )
add_subdirectory( toolbox     ${CMAKE_CURRENT_BINARY_DIR}/renderboi.toolbox )

if( RENDERBOI_BUILD_EXAMPLES )
    add_subdirectory( examples ${CMAKE_CURRENT_BINARY_DIR}/renderboi.examples )
endif( )

set( RENDERBOI_MAIN_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_library( renderboi INTERFACE )

target_include_directories( renderboi INTERFACE ${RENDERBOI_MAIN_INCLUDE_PATH} )
target_link_libraries( renderboi INTERFACE
    ${CMAKE_DL_LIBS}
    renderboi_core
    renderboi_utilities
    renderboi_window
    renderboi_toolbox
) 
