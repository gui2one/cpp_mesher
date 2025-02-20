cmake_minimum_required(VERSION 3.27.0)

set(VCPKG_ROOT $ENV{VCPKG_ROOT})

if(NOT VCPKG_ROOT)
    message(FATAL_ERROR "The environment variable VCPKG_ROOT is not set.")
endif()


##try and disable installation beacuse of they should be installed already in vcpkg global

# set (VCPKG_FEATURE_FLAGS "manifests,manifests-root,use-global-config")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set (VCPKG_DOWNLOADS "${VCPKG_ROOT}/downloads")
set(VCPKG_MANIFEST_INSTALL ON)

set(FETCHCONTENT_UPDATES_DISCONNECTED OFF) 

set(FETCHCONTENT_QUIET OFF)
include(FetchContent)  

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS /w /wnoerror /EHsc)
project(cpp_mesher)

# Define the source directory
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

# Recursively collect all .cpp and .h files
file(GLOB_RECURSE SOURCES
    "${SOURCE_DIR}/*.cpp"
    "${SOURCE_DIR}/*.h"
)
# Organize files into folders in Visual Studio
foreach(FILE_PATH IN LISTS SOURCES)
    # Get the directory relative to the source directory
    file(RELATIVE_PATH REL_PATH "${SOURCE_DIR}" "${FILE_PATH}")
    
    # Extract the directory part of the relative path
    get_filename_component(FOLDER_PATH "${REL_PATH}" DIRECTORY)

    # Replace "/" with "\\" to ensure compatibility with Visual Studio
    string(REPLACE "/" "\\" FOLDER_PATH "src/${FOLDER_PATH}")

    # Assign the file to a source group
    source_group("${FOLDER_PATH}" FILES "${FILE_PATH}")
endforeach()

add_executable(${PROJECT_NAME} 

    ${SOURCES}
)
add_compile_options(/O2 /Ob3 /fp:fast /arch:AVX2)

target_precompile_headers(${PROJECT_NAME} PUBLIC ./src/cpp_mesher.h ./src/pch.h)
set(CMAKE_BUILD_TYPE $<IF:$<CONFIG:Debug>,Debug,Release> CACHE STRING "Choose the type of build." FORCE)
target_link_directories(${PROJECT_NAME} PUBLIC 
   $<$<CONFIG:Debug>:${VCPKG_ROOT}/installed/x64-windows/debug/lib>
   $<$<CONFIG:Release>:${VCPKG_ROOT}/installed/x64-windows/lib>
)
target_include_directories(${PROJECT_NAME} PUBLIC $ENV{VCPKG_ROOT}/installed/x64-windows/include)

function(FetchDependencyAndMakeAvailable NAME URL TAG)
  FetchContent_Declare(
    ${NAME}
    GIT_REPOSITORY ${URL}
    GIT_TAG        ${TAG}
    GIT_PROGRESS TRUE
  )
  FetchContent_MakeAvailable(${NAME})
endfunction()
function(CopyDLL TARGET_NAME DLL_NAME)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/build/_deps/glfw-build/src/$<IF:$<CONFIG:Debug>,Debug/glfw3d.dll,Release/glfw3.dll>"
        "${CMAKE_BINARY_DIR}/$<IF:$<CONFIG:Debug>,Debug,Release>/"
    )
endfunction()

set("include_dirs"
    ${VCPKG_ROOT}/installed/x64-windows/include
    src/
    src/opengl_renderer/
    vendor/
    build/_deps/glm-src
    build/_deps/spdlog-src/include
    build/_deps/node_editor-src/src
    build/_deps/yaml-cpp-src/include
    # build/_deps/opensubdiv-src
    build/_deps/openmesh-src/src
    build/_deps/shader_include-src/
)
target_include_directories(${PROJECT_NAME} PUBLIC ${include_dirs})
include_directories( ${include_dirs})

# ZLIB
find_package(ZLIB REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC ZLIB::ZLIB)
# CGAL
# set(CMAKE_BUILD_TYPE "Release")
find_package(cgal CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE CGAL::CGAL)

# ASSIMP
find_package(assimp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp::assimp)

#opensubdiv from vcpkg
find_package(OpenSubdiv CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE osdCPU)

# # OpenSubdiv
# # Set the desired options for OpenSubdiv
# set(NO_PTEX ON CACHE BOOL "Disable PTex support" FORCE)
# set(NO_DOC ON CACHE BOOL "Disable documentation generation" FORCE)
# set(NO_OMP ON CACHE BOOL "Disable OpenMP support" FORCE)
# set(NO_TBB ON CACHE BOOL "Disable TBB support" FORCE)
# set(NO_CUDA ON CACHE BOOL "Disable CUDA support" FORCE)
# set(NO_OPENCL ON CACHE BOOL "Disable OpenCL support" FORCE)
# set(NO_CLEW ON CACHE BOOL "Disable CLEW support" FORCE)
# #set(NO_TUTORIALS ON CACHE BOOL "Disable tutorials build" FORCE)
# #set(NO_EXAMPLES ON CACHE BOOL "Disable examples build" FORCE)
# #set(NO_REGRESSION ON CACHE BOOL "Disable regression tests build" FORCE)
# add_definitions(
#     -DBUILD_SHARED_LIBS=1
#     -DNO_REGRESSION=1
#     -DNO_EXAMPLES=1
# )
# FetchDependencyAndMakeAvailable(opensubdiv https://github.com/PixarAnimationStudios/OpenSubdiv 7d0ab5530feef693ac0a920585b5c663b80773b3)
# target_compile_options(osd_static_cpu PRIVATE /EHsc)
# target_link_libraries(${PROJECT_NAME} PUBLIC osd_static_cpu)


# OpenMesh
set( BUILD_APPS false CACHE BOOL "Enable or disable building of apps" )
# set( DISABLE_QMAKE_BUILD true CACHE BOOL "Disable QMake Build" )
FetchDependencyAndMakeAvailable(OpenMesh https://github.com/Lawrencemm/openmesh 4e2e481f438747d64e62899021da9f469fd9daf8)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenMeshCore)


# FastNoise2
FetchDependencyAndMakeAvailable(FastNoise https://github.com/Auburn/FastNoise2 9937723493bb6fd1b8f1491bf9ce251a8867226b)
target_link_libraries(${PROJECT_NAME} PUBLIC FastNoise)
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
# COMMAND ${CMAKE_COMMAND} -E copy_if_different
# "${PROJECT_SOURCE_DIR}/build/_deps/fastnoise-build/src/$<IF:$<CONFIG:Debug>,Debug/FastNoised.dll,Release/FastNoise.dll>"
# $<TARGET_FILE_DIR:${PROJECT_NAME}>)

# GLM
FetchDependencyAndMakeAvailable(glm https://github.com/g-truc/glm 33b4a621a697a305bc3a7610d290677b96beb181)


# TINYGLTF
set(TINYGLTF_HEADER_ONLY ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF CACHE INTERNAL "" FORCE)
FetchDependencyAndMakeAvailable(tinygltf https://github.com/syoyo/tinygltf fda7422022ff058b1b7b501d373a97b1cc377bfd)

include_directories(build/_deps/tinygltf-src)

# SPDLOG
FetchDependencyAndMakeAvailable(spdlog https://github.com/gabime/spdlog e593f6695c6065e6b345fe2862f04a519ed484e0)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog ${SPDLOG_LIBRAIRIES})

# add_custom_command(
#     TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#         "${PROJECT_SOURCE_DIR}/build/_deps/spdlog-build/$<IF:$<CONFIG:Debug>,Debug/spdlogd.dll,Release/spdlog.dll>"
#         "${CMAKE_BINARY_DIR}/$<IF:$<CONFIG:Debug>,Debug,Release>/"
# )


# set(FETCHCONTENT_UPDATES_DISCONNECTED ON) 
# MY node_editor !!
set(NODE_EDITOR_BUILD_EXAMPLE OFF)
set(NODE_EDITOR_BUILD_DOC OFF)
# add_definitions(-DMODERN_OGL_DEBUG)
FetchDependencyAndMakeAvailable(
    node_editor
    https://github.com/gui2one/node-editor
    master
    )
    
target_link_libraries(${PROJECT_NAME} PUBLIC node_editor)
target_include_directories(${PROJECT_NAME} PUBLIC build/_deps/node_editor-src/src)
# set(FETCHCONTENT_UPDATES_DISCONNECTED OFF) 


### copy Node-Editor resources folder
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/build/_deps/node_editor-src/resources" "${CMAKE_CURRENT_BINARY_DIR}$<IF:$<CONFIG:Debug>,/Debug,/Release>/resources"
)
### copy mesher resources folder
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/mesher_resources" "${CMAKE_CURRENT_BINARY_DIR}$<IF:$<CONFIG:Debug>,/Debug,/Release>/mesher_resources"
)

# YAML-CPP
set(YAML_CPP_BUILD_TOOLS OFF)
set(YAML_BUILD_SHARED_LIBS ON)
FetchDependencyAndMakeAvailable(yaml-cpp https://github.com/jbeder/yaml-cpp da82fd982c260e7f335ce5acbceff24b270544d1) #master 10/11/2024
target_link_libraries(${PROJECT_NAME} PUBLIC yaml-cpp::yaml-cpp)
target_include_directories(${PROJECT_NAME} PUBLIC build/_deps/yaml-cpp-src/include)


# shader include
FETCHDEPENDENCYANDMAKEAVAILABLE(
    shader_include
    https://github.com/tntmeijs/GLSL-Shader-Includes.git
    8e4963b7521bbb5478597c9b8ab880d44223cfe8 #master 13 feb 2025
)