cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(cpp_mesher)


add_executable(${PROJECT_NAME} 

    src/tinygltf.cpp
    src/stb_image.cpp
    src/stb_image_write.cpp

    src/main.cpp
    src/utils.cpp
    src/gltf_utils.cpp
    src/Mesh.cpp
    src/Log.cpp
    src/OperatorNetwork.cpp
    src/MeshImporter.cpp
    src/MeshExporter.cpp
    src/meshop/IMeshOperator.cpp
    src/meshop/MeshGenerator.cpp
    src/meshop/generators/GridGenerator.cpp
    src/meshop/generators/SquareGenerator.cpp
)

target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.h)

set("include_dirs"
    src/
    vendor/
    vendor/glm
    vendor/assimp/include
    vendor/spdlog/include
    vendor/tinygltf
)
target_include_directories(${PROJECT_NAME} PUBLIC ${include_dirs})
# include_directories( ${include_dirs})

# ASSIMP
add_subdirectory(vendor/assimp)
target_link_libraries(${PROJECT_NAME} assimp ${ASSIMP_LIBRARIES})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/build/vendor/assimp/bin/$<IF:$<CONFIG:Debug>,Debug/assimp-vc142-mtd.dll,Release/assimp-vc142-mt.dll>"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

# TINYGLTF
set(TINYGLTF_HEADER_ONLY ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL ON CACHE INTERNAL "" FORCE)
# set(TINYGLTF_NO_EXTERNAL_IMAGE ON CACHE INTERNAL "" FORCE)
# set(TINYGLTF_NO_INCLUDE_STB_IMAGE ON CACHE INTERNAL "" FORCE)
# set(TINYGLTF_NO_INCLUDE_STB_IMAGE_WRITE ON CACHE INTERNAL "" FORCE)
# set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF CACHE INTERNAL "" FORCE)

add_subdirectory(vendor/tinygltf)
include_directories(vendor/tinygltf)
# target_link_libraries(${PROJECT_NAME} tinygltf ${TINYGLTF_LIBRAIRIES})