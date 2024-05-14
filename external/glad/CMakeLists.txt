add_library( glad STATIC
    gl.c
    gl.h
    KHR/khrplatform.h
)

set_target_properties(
    glad PROPERTIES
    LINKER_LANGUAGE C
)

target_include_directories( glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. )
