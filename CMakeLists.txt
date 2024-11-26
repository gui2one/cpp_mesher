cmake_minimum_required(VERSION 3.27.0)

if(NOT DEFINED ENV{VCPKG_ROOT})
    message(FATAL_ERROR "The environment variable VCPKG_ROOT is not set.")
endif()

##try and disable installation beacuse of they should be installed already in vcpkg global
set(VCPKG_MANIFEST_INSTALL OFF)

set (VCPKG_FEATURE_FLAGS "manifests,manifests-root,use-global-config")
set (VCPKG_DOWNLOADS "c:/vcpkg/downloads")
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

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
    # src/main.cpp
    # src/formatters.cpp
    # src/Application.cpp
    # src/MeshExporter.cpp
    # src/MeshImporter.cpp
    # src/Log.cpp
    # src/Mesh.cpp
    # src/mesh_utils.cpp
    # src/cgal/boolean_utils.cpp
    # 
    # src/nodes/MeshOperators.cpp
    # src/openmesh/openmesh_operators.cpp
    # src/openmesh/openmesh_utils.cpp
    # src/openmesh/gmesh.cpp
)

target_precompile_headers(${PROJECT_NAME} PUBLIC ./src/cpp_mesher.h ./src/pch.h)
target_link_directories(${PROJECT_NAME} PUBLIC $ENV{VCPKG_ROOT}/installed/x64-windows/lib)
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
    C:/vcpkg/installed/x64-windows/include
    src/
    vendor/
    build/_deps/glm-src
    build/_deps/spdlog-src/include
    build/_deps/node_editor-src/src
    build/_deps/yaml-cpp-src/include
    build/_deps/opensubdiv-src
    build/_deps/openmesh-src/src
)
target_include_directories(${PROJECT_NAME} PUBLIC ${include_dirs})
include_directories( ${include_dirs})

# ZLIB
find_package(ZLIB REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC ZLIB::ZLIB)
# CGAL
set(CMAKE_BUILD_TYPE "Release")
find_package(cgal CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE CGAL::CGAL)

# ASSIMP
find_package(assimp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp::assimp)

# OpenSubdiv
# Set the desired options for OpenSubdiv
set(NO_PTEX 1 CACHE BOOL "Disable PTex support")
set(NO_DOC 1 CACHE BOOL "Disable documentation generation")
set(NO_OMP 1 CACHE BOOL "Disable OpenMP support")
set(NO_TBB 1 CACHE BOOL "Disable TBB support")
set(NO_CUDA 1 CACHE BOOL "Disable CUDA support")
set(NO_OPENCL 1 CACHE BOOL "Disable OpenCL support")
set(NO_CLEW 1 CACHE BOOL "Disable CLEW support")
set(NO_TUTORIALS 1 CACHE BOOL "Disable tutorials build")
set(NO_EXAMPLES 1 CACHE BOOL "Disable examples build")
set(NO_REGRESSION 1 CACHE BOOL "Disable regression tests build")

FetchDependencyAndMakeAvailable(opensubdiv https://github.com/PixarAnimationStudios/OpenSubdiv 7d0ab5530feef693ac0a920585b5c663b80773b3)
target_compile_options(osd_static_cpu PRIVATE /EHsc)
target_link_libraries(${PROJECT_NAME} PUBLIC osd_static_cpu)


# OpenMesh
set( BUILD_APPS false CACHE BOOL "Enable or disable building of apps" )
# set( DISABLE_QMAKE_BUILD true CACHE BOOL "Disable QMake Build" )
FetchDependencyAndMakeAvailable(OpenMesh https://github.com/Lawrencemm/openmesh 4e2e481f438747d64e62899021da9f469fd9daf8)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenMeshCore)


# FastNoise2
FetchDependencyAndMakeAvailable(FastNoise https://github.com/Auburn/FastNoise2 9937723493bb6fd1b8f1491bf9ce251a8867226b)
target_link_libraries(${PROJECT_NAME} PUBLIC FastNoise)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different
"${PROJECT_SOURCE_DIR}/build/_deps/fastnoise-build/src/$<IF:$<CONFIG:Debug>,Debug/FastNoised.dll,Release/FastNoise.dll>"
$<TARGET_FILE_DIR:${PROJECT_NAME}>)

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

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/build/_deps/spdlog-build/$<IF:$<CONFIG:Debug>,Debug/spdlogd.dll,Release/spdlog.dll>"
        "${CMAKE_BINARY_DIR}/$<IF:$<CONFIG:Debug>,Debug,Release>/"
)


# set(FETCHCONTENT_UPDATES_DISCONNECTED ON) 
# MY node_editor !!
set(NODE_EDITOR_BUILD_EXAMPLE OFF)
set(NODE_EDITOR_BUILD_DOC OFF)
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
# add_custom_command(
#     TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#         "${PROJECT_SOURCE_DIR}/build/_deps/yaml-cpp-build/$<IF:$<CONFIG:Debug>,Debug/yaml-cppd.dll,Release/yaml-cpp.dll>"
#         "${CMAKE_BINARY_DIR}/$<IF:$<CONFIG:Debug>,Debug,Release>/"
# )