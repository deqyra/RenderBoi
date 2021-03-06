# Generate script file to copy headers
file(WRITE ${CMAKE_BINARY_DIR}/cp_${RB_TOOLBOX_LIB_NAME}_headers.cmake
    "file(COPY ${CMAKE_CURRENT_SOURCE_DIR}
        DESTINATION ${EXPORT_LOCATION}/include/renderboi
        FILES_MATCHING
        PATTERN \"*.hpp\"
    )\n"
)

# Build toolbox lib
add_library(${RB_TOOLBOX_LIB_NAME} ${STATIC_OR_SHARED}
    ${RB_TOOLBOX_SOURCE_FILES}
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
