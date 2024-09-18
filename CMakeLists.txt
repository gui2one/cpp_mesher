cmake_minimum_required(VERSION 3.20.0)

project(cpp_mesher)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
    vendor/glm
)