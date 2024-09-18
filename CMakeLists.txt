cmake_minimum_required(VERSION 3.20.0)

project(cpp_mesher)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
    vendor/glm
    vendor/assimp/include
    vendor/assimp
)

# ASSIMP
add_subdirectory(vendor/assimp)
# target_include_directories(${PROJECT_NAME} PUBLIC vendor/assimp/include)
target_link_libraries(${PROJECT_NAME} assimp ${ASSIMP_LIBRARIES})