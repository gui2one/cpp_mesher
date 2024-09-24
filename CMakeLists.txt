cmake_minimum_required(VERSION 3.27.0)


include(FetchContent)   
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(cpp_mesher)


add_executable(${PROJECT_NAME} 

    src/tinygltf.cpp
    src/stb_image.cpp
    src/stb_image_write.cpp

    src/main.cpp
    src/formatters.cpp
    src/utils.cpp
    src/gltf_utils.cpp
    src/Mesh.cpp
    src/Log.cpp
    src/OperatorNetwork.cpp
    src/MeshImporter.cpp
    src/MeshExporter.cpp
    
    src/meshop/IMeshOperator.cpp
    # src/meshop/MeshGenerator.cpp
    # src/meshop/MeshModifier.cpp
    
    src/meshop/modifiers/NormalModifier.cpp
    
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
include_directories( ${include_dirs})

# ASSIMP
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_PLY_EXPORTER ON CACHE INTERNAL "" FORCE)

set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_PLY_IMPORTER ON CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_OFF_IMPORTER ON CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE INTERNAL "" FORCE)

FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp
  GIT_TAG        17399d198f6411dfcd25a4adb4c3a40733f0a717) # master
FetchContent_MakeAvailable(assimp)


# add_subdirectory(vendor/assimp)
target_link_libraries(${PROJECT_NAME} assimp::assimp ${ASSIMP_LIBRARIES})
message("ASSIMP_LIBRARIES: ${ASSIMP_LIBRARIES}")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/build/vendor/assimp/bin/$<IF:$<CONFIG:Debug>,Debug/assimp-vc142-mtd.dll,Release/assimp-vc142-mt.dll>"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

# TINYGLTF
set(TINYGLTF_HEADER_ONLY ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF CACHE INTERNAL "" FORCE)
# set(TINYGLTF_NO_EXTERNAL_IMAGE ON CACHE INTERNAL "" FORCE)
# set(TINYGLTF_NO_INCLUDE_STB_IMAGE ON CACHE INTERNAL "" FORCE)
# set(TINYGLTF_NO_INCLUDE_STB_IMAGE_WRITE ON CACHE INTERNAL "" FORCE)

add_subdirectory(vendor/tinygltf)
include_directories(vendor/tinygltf)
# target_link_libraries(${PROJECT_NAME} tinygltf ${TINYGLTF_LIBRAIRIES})


FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        e69e5f977d458f2650bb346dadf2ad30c5320281) # 10.2.1
FetchContent_MakeAvailable(fmt)

target_link_libraries(${PROJECT_NAME} fmt::fmt)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/build/_deps/fmt-build/bin/$<IF:$<CONFIG:Debug>,Debug/fmtd.dll,Release/fmt.dll>"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)




option(SPDLOG_USE_STD_FORMAT OFF)
add_subdirectory(vendor/spdlog)

# Get a list of all targets in the current scope
get_property(all_targets DIRECTORY vendor/spdlog PROPERTY BUILDSYSTEM_TARGETS)

# Display the targets
message(STATUS "All targets in subdirectory: ${all_targets}")


target_link_libraries(${PROJECT_NAME} spdlog::spdlog ${SPDLOG_LIBRAIRIES})
message("SPDLOG_LIBRARIES: ${SPDLOG_LIBRAIRIES}")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/build/vendor/spdlog/$<IF:$<CONFIG:Debug>,Debug/spdlogd.dll,Release/spdlog.dll>"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)




