add_library( stb_image
    stb_image.cpp
)

add_dependencies( stb_image
    stb
)

target_include_directories( stb_image PUBLIC
    ${STB_SOURCE_DIR}
)