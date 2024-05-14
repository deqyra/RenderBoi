# Build examples
add_executable( renderboi_examples
    gl_sandbox_parameters.hpp
    gl_sandbox.cpp
    gl_sandbox.hpp
    gl_sandbox_runner.hpp
    lighting_sandbox.cpp
    lighting_sandbox.hpp
    main.cpp
    project_env.hpp
    renderboi_parameters.hpp
    # shadow_sandbox.cpp
    # shadow_sandbox.hpp
)

target_include_directories( renderboi_examples PUBLIC ${RENDERBOI_MAIN_INCLUDE_PATH} )
target_link_libraries( renderboi_examples PUBLIC
    cpptools::cpptools_static
    glad
    renderboi_core
    renderboi_window
    renderboi_toolbox
)
