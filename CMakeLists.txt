###############################################################################
#                                                                             #
#                             CMake configuration                             #
#                                                                             #
###############################################################################

cmake_minimum_required(VERSION 3.0)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()



###############################################################################
#                                                                             #
#                                   Toggles                                   #
#                                                                             #
###############################################################################

option(BUILD_STATIC_LIBS "Build libraries as static libraries" OFF)
option(BUILD_RELEASE "Whether to build for a debug or release profile" OFF)

if(${BUILD_STATIC_LIBS})
    set(STATIC_OR_SHARED "STATIC")
    set(BUILD_SHARED_LIBS TRUE)
else()
    set(STATIC_OR_SHARED "SHARED")
endif()

if (${BUILD_RELEASE})
    set(CMAKE_BUILD_TYPE "Release")
else()
    set(CMAKE_BUILD_TYPE "Debug")
endif()

if(WIN32 AND (STATIC_OR_SHARED STREQUAL "SHARED"))
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()



###############################################################################
#                                                                             #
#                              Compiler options                               #
#                                                                             #
###############################################################################

set(CMAKE_CXX_STANDARD 20)

if (NOT ${BUILD_RELEASE})
    if ($<COMPILER_CXX_ID:Clang> MATCHES "Clang")
        add_definitions(-D_SILENCE_CLANG_CONCEPTS_MESSAGE)
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -O0")
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Intel")
        # using Intel C++
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /Od")
    endif()
endif()

if(CMAKE_COMPILER_IS_GNUCC)
endif()



###############################################################################
#                                                                             #
#                             Project definition                              #
#                                                                             #
###############################################################################

# Project general info
set(PROJECT_VERSION "1.0")
set(PROJECT_NAME "RenderBoi")

project(
    ${PROJECT_NAME}
    VERSION ${PROJECT_VERSION}
    DESCRIPTION "A rendering engine with a modest feature set"
    LANGUAGES C CXX
)

set(PROJECT_MAINTAINER_NAME "François Brachais")
set(PROJECT_MAINTAINER_EMAIL "deqyra@gmail.com")

# GL loader information
set(GL_CONTEXT_VERSION_MAJOR "4")
set(GL_CONTEXT_VERSION_MINOR "2")

# Copyleft notice generation
set(PROJECT_START_YEAR "2019")
set(DATE_SPAN "${PROJECT_START_YEAR}")

string(TIMESTAMP CURRENT_YEAR "%Y")
if(NOT CURRENT_YEAR EQUAL PROJECT_START_YEAR)
    set(DATE_SPAN "${PROJECT_START_YEAR}-${CURRENT_YEAR}")
endif()

set(PROJECT_COPYRIGHT "${DATE_SPAN} © ${PROJECT_MAINTAINER_NAME} <${PROJECT_MAINTAINER_EMAIL}>")

# File configuration
configure_file("${CMAKE_SOURCE_DIR}/renderboi/config/project_env.config.hpp" "${CMAKE_SOURCE_DIR}/renderboi/project_env.hpp")



###############################################################################
#                                                                             #
#                            Project-wide toggles                             #
#                                                                             #
###############################################################################

option(WINDOW_BACKEND_GLFW3 "Use GLFW3 as the window backend" ON)
option(
    GLFW3_BORDERLESS_POLICY_NATIVE
    "Have GLFW3 detect borderless fullscreen parameters upon starting up
    (default, overrides GLFW3_BORDERLESS_POLICY_MAX_MODE)"
    ON
)
option(
    GLFW3_BORDERLESS_POLICY_MAX_MODE
    "Have GLFW3 use the largest available video mode for borderless fullscreen"
    OFF
)



###############################################################################
#                                                                             #
#                            Dependency locations                             #
#                                                                             #
###############################################################################

set(EXPORT_LOCATION ${CMAKE_SOURCE_DIR}/external)
set(EXTERNAL_SOURCE_LOCATION ${CMAKE_SOURCE_DIR}/external/source)
set(RENDERBOI_MODULE_LOCATION ${CMAKE_SOURCE_DIR}/renderboi)
file(MAKE_DIRECTORY ${EXPORT_LOCATION})
file(MAKE_DIRECTORY ${EXPORT_LOCATION}/include)
file(MAKE_DIRECTORY ${EXPORT_LOCATION}/lib)



###############################################################################
#                                                                             #
#                                Target names                                 #
#                                                                             #
###############################################################################

# cpptools
set(CPPTOOLS_LIB_NAME "cpptools")

include(FetchContent)
FetchContent_Declare(${CPPTOOLS_LIB_NAME}
    GIT_REPOSITORY https://github.com/deqyra/CppTools.git
)

set(CPPTOOLS_EXPORT_LOCATION ${EXPORT_LOCATION})

FetchContent_GetProperties(${CPPTOOLS_LIB_NAME})
if(NOT cpptools_POPULATED)
  FetchContent_Populate(${CPPTOOLS_LIB_NAME})
  add_subdirectory(${cpptools_SOURCE_DIR} ${cpptools_BINARY_DIR})
endif()

# Everything else
set(GLAD_LIB_NAME "glad")
set(GETOPT_WIN_LIB_NAME "getopt")
set(STB_IMAGE_LIB_NAME "stb_image")
set(GLM_LIB_NAME "glm")
set(RB_UTILITIES_LIB_NAME "renderboi.utilities")
set(RB_CORE_LIB_NAME "renderboi.core")
set(RB_WINDOW_LIB_NAME "renderboi.window")
set(RB_TOOLBOX_LIB_NAME "renderboi.toolbox")
set(RB_EXAMPLES_LIB_NAME "renderboi.examples")


###############################################################################
#                                                                             #
#                           Include subdirectories                            #
#                                                                             #
###############################################################################

# Import getopt target only for windows (linux already has it)
if(WIN32)
    add_subdirectory(${EXTERNAL_SOURCE_LOCATION}/getoptwin ${CMAKE_BINARY_DIR}/${GETOPT_WIN_LIB_NAME})
endif()

# Import GLAD targets
add_subdirectory(${EXTERNAL_SOURCE_LOCATION}/glad ${CMAKE_BINARY_DIR}/${GLAD_LIB_NAME})

# Import stb_image targets
add_subdirectory(${EXTERNAL_SOURCE_LOCATION}/stb_image ${CMAKE_BINARY_DIR}/${STB_IMAGE_LIB_NAME})

# Import GLM targets
add_subdirectory(${EXTERNAL_SOURCE_LOCATION}/glm ${CMAKE_BINARY_DIR}/${GLM_LIB_NAME})

# Import RenderBoi.Utilities targets
add_subdirectory(${RENDERBOI_MODULE_LOCATION}/utilities ${CMAKE_BINARY_DIR}/${RB_UTILITIES_LIB_NAME})

# Import RenderBoi.Core targets
add_subdirectory(${RENDERBOI_MODULE_LOCATION}/core ${CMAKE_BINARY_DIR}/${RB_CORE_LIB_NAME})

# Import RenderBoi.Window targets
add_subdirectory(${RENDERBOI_MODULE_LOCATION}/window ${CMAKE_BINARY_DIR}/${RB_WINDOW_LIB_NAME})

# Import RenderBoi.Toolbox targets
add_subdirectory(${RENDERBOI_MODULE_LOCATION}/toolbox ${CMAKE_BINARY_DIR}/${RB_TOOLBOX_LIB_NAME})

# Import RenderBoi.Examples targets
add_subdirectory(${RENDERBOI_MODULE_LOCATION}/examples ${CMAKE_BINARY_DIR}/${RB_EXAMPLES_LIB_NAME})



###############################################################################
#                                                                             #
#                         Build RenderBoi executable                          #
#                                                                             #
###############################################################################

# Build executable
add_executable("RenderBoi"
    ${RENDERBOI_MODULE_LOCATION}/main.cpp
    ${RENDERBOI_MODULE_LOCATION}/main_functions.hpp
    ${RENDERBOI_MODULE_LOCATION}/main_functions.cpp
    ${RENDERBOI_MODULE_LOCATION}/project_env.hpp
    ${RENDERBOI_MODULE_LOCATION}/renderboi_parameters.hpp
)   

add_dependencies("RenderBoi"
    "export_${RB_UTILITIES_LIB_NAME}_lib"
    "export_${RB_UTILITIES_LIB_NAME}_headers"
    "export_${RB_WINDOW_LIB_NAME}_lib"
    "export_${RB_WINDOW_LIB_NAME}_headers"
    "export_${RB_EXAMPLES_LIB_NAME}_lib"
    "export_${RB_EXAMPLES_LIB_NAME}_headers"
)

if(WIN32)
    add_dependencies("RenderBoi"
        "export_${GETOPT_WIN_LIB_NAME}_lib"
        "export_${GETOPT_WIN_LIB_NAME}_headers"
    )
endif()

target_include_directories("RenderBoi" PUBLIC ${EXPORT_LOCATION}/include)
target_link_directories("RenderBoi" PUBLIC ${EXPORT_LOCATION}/lib)

target_link_libraries("RenderBoi" PUBLIC ${CMAKE_DL_LIBS}
    ${RB_WINDOW_LIB_NAME}
    ${RB_EXAMPLES_LIB_NAME}
) 

if(WIN32)
    target_link_libraries("RenderBoi" PUBLIC ${CMAKE_DL_LIBS}
        ${GETOPT_WIN_LIB_NAME}
    ) 
endif()

add_custom_command(TARGET "RenderBoi" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${EXPORT_LOCATION}/lib" $<TARGET_FILE_DIR:RenderBoi>
)



###############################################################################
#                                                                             #
#                                    Tests                                    #
#                                                                             #
###############################################################################

set(TEST_SOURCE_LOCATION ${CMAKE_SOURCE_DIR}/tests)
set(TEST_SOURCE_FILES
    ${TEST_SOURCE_LOCATION}/main.cpp
)

# Build and run tests
enable_testing()
add_executable("RenderBoiTests"
    ${TEST_SOURCE_FILES}
)

target_include_directories("RenderBoiTests" PUBLIC ${EXPORT_LOCATION}/include)
target_link_directories("RenderBoiTests" PUBLIC ${EXPORT_LOCATION}/lib)

target_link_libraries("RenderBoiTests" PUBLIC ${CMAKE_DL_LIBS}
    ${RB_CORE_LIB_NAME}
    # ${RB_TOOLBOX_LIB_NAME}
    # ${RB_WINDOW_LIB_NAME}
) 

add_test(NAME tests 
    COMMAND "RenderBoiTests"
)
