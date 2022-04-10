# Generate script file to copy headers
file(WRITE ${CMAKE_BINARY_DIR}/cp_${RB_CORE_LIB_NAME}_headers.cmake
    "file(COPY \"${CMAKE_CURRENT_SOURCE_DIR}\"
        DESTINATION \"${EXPORT_LOCATION}/include/renderboi\"
        FILES_MATCHING
        PATTERN \"*.hpp\"
    )\n"
)

# Build core lib
add_library(${RB_CORE_LIB_NAME} ${STATIC_OR_SHARED}
    camera.cpp
    camera.hpp
    material.cpp
    material.hpp
    materials.hpp
    mesh.cpp
    mesh.hpp
    pixel_space.hpp
    transform.cpp
    transform.hpp
    texture_2d.cpp
    texture_2d.hpp
    vertex.hpp
    common/parent_dependent_vp_matrix_provider.hpp
    common/parent_dependent_vp_matrix_provider.cpp
    interfaces/basis_provider.hpp
    interfaces/vp_matrix_provider.hpp
    lights/directional_light.cpp
    lights/directional_light.hpp
    lights/light.cpp
    lights/light.hpp
    lights/light_type.hpp
    lights/point_light.cpp
    lights/point_light.hpp
    lights/spot_light.cpp
    lights/spot_light.hpp 
    lights/tools.cpp
    lights/tools.hpp 
    shader/shader.cpp
    shader/shader.hpp
    shader/shader_builder.cpp
    shader/shader_builder.hpp
    shader/shader_config.cpp
    shader/shader_config.hpp
    shader/shader_feature.cpp
    shader/shader_feature.hpp
    shader/shader_program.cpp
    shader/shader_program.hpp
    shader/shader_stage.cpp
    shader/shader_stage.hpp
    ubo/light_ubo.cpp
    ubo/light_ubo.hpp 
    ubo/matrix_ubo.cpp
    ubo/matrix_ubo.hpp 
)
add_dependencies(${RB_CORE_LIB_NAME}
    "export_${GLM_LIB_NAME}_headers"
    "export_${CPPTOOLS_LIB_NAME}_headers"
    "export_${CPPTOOLS_LIB_NAME}_lib"
    "export_${GLAD_LIB_NAME}_lib"
    "export_${GLAD_LIB_NAME}_headers"
    "export_${STB_IMAGE_LIB_NAME}_lib"
    "export_${STB_IMAGE_LIB_NAME}_headers"
    "export_${RB_UTILITIES_LIB_NAME}_lib"
    "export_${RB_UTILITIES_LIB_NAME}_headers"
)
target_include_directories(${RB_CORE_LIB_NAME} PUBLIC ${EXPORT_LOCATION}/include)
target_link_directories(${RB_CORE_LIB_NAME} PUBLIC ${EXPORT_LOCATION}/lib)
target_link_libraries(${RB_CORE_LIB_NAME} PUBLIC ${CMAKE_DL_LIBS}
    ${STB_IMAGE_LIB_NAME}
    ${GLAD_LIB_NAME}
    ${CPPTOOLS_LIB_NAME}
    ${RB_UTILITIES_LIB_NAME}
)
add_compile_definitions(GLAD_GLAPI_EXPORT)

# Custom target to export core lib
add_custom_target("export_${RB_CORE_LIB_NAME}_lib")
add_dependencies("export_${RB_CORE_LIB_NAME}_lib" ${RB_CORE_LIB_NAME})
add_custom_command(
    TARGET "export_${RB_CORE_LIB_NAME}_lib"
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${RB_CORE_LIB_NAME}> ${EXPORT_LOCATION}/lib/
)

# Custom target to export core headers
add_custom_target("export_${RB_CORE_LIB_NAME}_headers")
add_custom_command(
    TARGET "export_${RB_CORE_LIB_NAME}_headers"
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cp_${RB_CORE_LIB_NAME}_headers.cmake
)
