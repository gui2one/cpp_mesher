cmake_minimum_required(VERSION 3.27.0)


include(FetchContent)   
set(FETCHCONTENT_QUIET FALSE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(cpp_mesher)


add_executable(${PROJECT_NAME} 

    src/main.cpp
    src/Application.cpp
    src/MeshExporter.cpp
    src/Log.cpp
    src/Mesh.cpp
    src/mesh_utils.cpp
)

target_precompile_headers(${PROJECT_NAME} PUBLIC ./src/cpp_mesher.h ./src/pch.h)

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
    src/
    vendor/
    build/_deps/glm-src
    build/_deps/assimp-build/include
    build/_deps/assimp-src/include
    build/_deps/spdlog-src/include
    build/_deps/node_editor-src/src
    build/_deps/yaml-cpp-src/include
)
target_include_directories(${PROJECT_NAME} PUBLIC ${include_dirs})
include_directories( ${include_dirs})

# FastNoise2
FetchDependencyAndMakeAvailable(FastNoise https://github.com/Auburn/FastNoise2 9937723493bb6fd1b8f1491bf9ce251a8867226b)
target_link_libraries(${PROJECT_NAME} PUBLIC FastNoise)

# GLM
FetchDependencyAndMakeAvailable(glm https://github.com/g-truc/glm 33b4a621a697a305bc3a7610d290677b96beb181)

# ASSIMP
set(BUILD_SHARED_LIBS ON)
set(ASSIMP_COVERALLS OFF)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_PLY_EXPORTER ON CACHE INTERNAL "" FORCE)

set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_PLY_IMPORTER ON CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_OFF_IMPORTER ON CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_SMD_IMPORTER ON CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_SIB_IMPORTER ON CACHE INTERNAL "" FORCE)

FetchDependencyAndMakeAvailable( assimp https://github.com/assimp/assimp ddb74c2bbdee1565dda667e85f0c82a0588c8053)

# target_link_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/build/_deps/assimp-build/lib/Release)
target_link_libraries(${PROJECT_NAME} PUBLIC assimp)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different
"${PROJECT_SOURCE_DIR}/build/_deps/assimp-build/bin/$<IF:$<CONFIG:Debug>,Debug/assimp-vc142-mtd.dll,Release/assimp-vc142-mt.dll>"
$<TARGET_FILE_DIR:${PROJECT_NAME}>)

# TINYGLTF
set(TINYGLTF_HEADER_ONLY ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF CACHE INTERNAL "" FORCE)
FetchDependencyAndMakeAvailable(tinygltf https://github.com/syoyo/tinygltf fda7422022ff058b1b7b501d373a97b1cc377bfd)

include_directories(build/_deps/tinygltf-src)

### FMT
FetchDependencyAndMakeAvailable(fmt https://github.com/fmtlib/fmt e69e5f977d458f2650bb346dadf2ad30c5320281)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/build/_deps/fmt-build/bin/$<IF:$<CONFIG:Debug>,Debug/fmtd.dll,Release/fmt.dll>"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

# SPDLOG
option(SPDLOG_USE_STD_FORMAT OFF)
FetchDependencyAndMakeAvailable(spdlog https://github.com/gabime/spdlog e593f6695c6065e6b345fe2862f04a519ed484e0)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog ${SPDLOG_LIBRAIRIES})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/build/_deps/spdlog-build/$<IF:$<CONFIG:Debug>,Debug/spdlogd.dll,Release/spdlog.dll>"
        "${CMAKE_BINARY_DIR}/$<IF:$<CONFIG:Debug>,Debug,Release>/"
)

# MY node_editor !!
set(NODE_EDITOR_BUILD_EXAMPLE OFF)
FetchDependencyAndMakeAvailable(
    node_editor
    https://github.com/gui2one/node-editor
    network-centric
)

target_link_libraries(${PROJECT_NAME} PUBLIC node_editor)
target_include_directories(${PROJECT_NAME} PUBLIC build/_deps/node_editor-src/src)


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