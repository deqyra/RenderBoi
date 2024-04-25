add_library( renderboi_utilities
    gl_utilities.cpp
    gl_utilities.hpp
    resource_locator.cpp
    resource_locator.hpp
)

target_include_directories( renderboi_utilities PUBLIC ${RENDERBOI_MAIN_INCLUDE_PATH} )
target_link_libraries( renderboi_utilities PUBLIC
    ${CMAKE_DL_LIBS}
    glad
    cpptools::cpptools
)
