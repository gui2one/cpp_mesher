cmake_minimum_required(VERSION 3.27.0)

project(cpp_mesher)

add_executable(${PROJECT_NAME} 

    src/main.cpp
    src/meshop/IMeshOperator.cpp
    src/meshop/MeshGenerator.cpp
)

target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.h)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
target_include_directories(${PROJECT_NAME} PUBLIC
    vendor/glm
    vendor/assimp/include
)

# ASSIMP
add_subdirectory(vendor/assimp)
target_link_libraries(${PROJECT_NAME} assimp ${ASSIMP_LIBRARIES})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/build/vendor/assimp/bin/$<IF:$<CONFIG:Debug>,Debug/assimp-vc142-mtd.dll,Release/assimp-vc142-mt.dll>"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
