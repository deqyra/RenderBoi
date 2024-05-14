################################################################################
#                                                                              #
#                                 Unit tests                                   #
#                                                                              #
################################################################################

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )

include( FetchContent )

FetchContent_Declare(
    Catch2
    DOWNLOAD_EXTRACT_TIMESTAMP ON
    URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.5.1.zip
)

FetchContent_MakeAvailable( Catch2 )
list( APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras )

include( CTest )
include( Catch )
enable_testing( )

add_executable( renderboi_tests
    main.cpp
)
target_include_directories( renderboi_tests PRIVATE
    ${CMAKE_SOURCE_DIR}
)
target_link_libraries( renderboi_tests PUBLIC
    ${CMAKE_DL_LIBS}
    renderboi
    Catch2::Catch2WithMain
) 

catch_discover_tests( renderboi_tests
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_test( NAME tests 
    COMMAND renderboi_tests
)
