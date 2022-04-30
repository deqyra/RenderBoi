# Generate script file to copy headers
file(WRITE ${CMAKE_BINARY_DIR}/cp_${RB_TOOLBOX_LIB_NAME}_headers.cmake
    "file(COPY \"${CMAKE_CURRENT_SOURCE_DIR}\"
        DESTINATION \"${EXPORT_LOCATION}/include/renderboi\"
        FILES_MATCHING
        PATTERN \"*.hpp\"
    )\n"
)

# Build toolbox lib
add_library(${RB_TOOLBOX_LIB_NAME} ${STATIC_OR_SHARED}
    common.hpp
    factory.hpp
    input_splitter.cpp
    input_splitter.hpp 
    script.cpp
    script.hpp 
    controls/control.cpp
    controls/control.hpp
    controls/control_event_translator.hpp
    controls/control_scheme.hpp
    controls/controlled_entity_manager.hpp
    interfaces/control_binding_provider.hpp
    interfaces/default_control_scheme_provider.hpp
    mesh_generators/all_mesh_generators.hpp
    mesh_generators/axes_generator.cpp
    mesh_generators/axes_generator.hpp
    mesh_generators/cube_generator.cpp
    mesh_generators/cube_generator.hpp
    mesh_generators/mesh_generator.hpp
    mesh_generators/plane_generator.cpp
    mesh_generators/plane_generator.hpp
    mesh_generators/tetrahedron_generator.cpp
    mesh_generators/tetrahedron_generator.hpp
    mesh_generators/torus_generator.cpp
    mesh_generators/torus_generator.hpp
    mesh_generators/mesh_type.hpp
    render/composite_renderer.hpp
    render/scene_renderer.cpp
    render/scene_renderer.hpp 
    render/traits/all_trait_renderers.hpp
    render/traits/render_trait.hpp
    render/traits/render_trait_config.cpp
    render/traits/render_trait_config.hpp
    render/traits/render_trait_config_map.cpp
    render/traits/render_trait_config_map.hpp
    render/traits/trait_renderer.hpp
    render/traits/config/mesh_render_trait_config.cpp
    render/traits/config/mesh_render_trait_config.hpp
    render/traits/renderers/mesh_trait_renderer.cpp
    render/traits/renderers/mesh_trait_renderer.hpp
    runnables/basic_window_manager.cpp
    runnables/basic_window_manager.hpp 
    runnables/camera_aspect_ratio_manager.cpp
    runnables/camera_aspect_ratio_manager.hpp 
    runnables/gamepad_movement_script.cpp
    runnables/gamepad_movement_script.hpp 
    runnables/gamepad_camera_manager.cpp
    runnables/gamepad_camera_manager.hpp 
    runnables/input_logger.cpp
    runnables/input_logger.hpp
    runnables/keyboard_movement_script.cpp
    runnables/keyboard_movement_script.hpp 
    runnables/mouse_camera_manager.cpp
    runnables/mouse_camera_manager.hpp 
    scene/scene.cpp
    scene/scene.hpp 
    scene/object/component.cpp
    scene/object/component.hpp 
    scene/object/component_map.cpp
    scene/object/component_map.hpp 
    scene/object/component_type.cpp 
    scene/object/component_type.hpp 
    scene/object/object_transform.cpp
    scene/object/object_transform.hpp
    scene/object/scene_object_metadata.hpp 
    scene/object/scene_object.cpp
    scene/object/scene_object.hpp 
    scene/object/components/all_components.cpp 
    scene/object/components/all_components.hpp 
    scene/object/components/camera_component.cpp
    scene/object/components/camera_component.hpp 
    scene/object/components/light_component.cpp
    scene/object/components/light_component.hpp 
    scene/object/components/mesh_component.cpp
    scene/object/components/mesh_component.hpp 
    scene/object/components/script_component.cpp
    scene/object/components/script_component.hpp 
)
add_dependencies(${RB_TOOLBOX_LIB_NAME}
    "export_${CPPTOOLS_LIB_NAME}_headers"
    "export_${CPPTOOLS_LIB_NAME}_lib"
    "export_${RB_CORE_LIB_NAME}_lib"
    "export_${RB_CORE_LIB_NAME}_headers"
    "export_${RB_WINDOW_LIB_NAME}_lib"
    "export_${RB_WINDOW_LIB_NAME}_headers"
    "export_${GLM_LIB_NAME}_headers"
)
target_include_directories(${RB_TOOLBOX_LIB_NAME} PUBLIC ${EXPORT_LOCATION}/include)
target_link_directories(${RB_TOOLBOX_LIB_NAME} PUBLIC ${EXPORT_LOCATION}/lib)
target_link_libraries(${RB_TOOLBOX_LIB_NAME} PUBLIC ${CMAKE_DL_LIBS}
    ${GLAD_LIB_NAME}
    ${RB_CORE_LIB_NAME}
    ${RB_WINDOW_LIB_NAME}
    ${CPPTOOLS_LIB_NAME}
)
add_compile_definitions(GLAD_GLAPI_EXPORT)

# Custom target to export toolbox lib
add_custom_target("export_${RB_TOOLBOX_LIB_NAME}_lib")
add_dependencies("export_${RB_TOOLBOX_LIB_NAME}_lib" ${RB_TOOLBOX_LIB_NAME})
add_custom_command(
    TARGET "export_${RB_TOOLBOX_LIB_NAME}_lib"
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${RB_TOOLBOX_LIB_NAME}> ${EXPORT_LOCATION}/lib
)

# Custom target to export toolbox headers
add_custom_target("export_${RB_TOOLBOX_LIB_NAME}_headers")
add_custom_command(
    TARGET "export_${RB_TOOLBOX_LIB_NAME}_headers"
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cp_${RB_TOOLBOX_LIB_NAME}_headers.cmake
)
