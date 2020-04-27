cmake_minimum_required(VERSION 3.2)
add_compile_options(-std=c++11)
project(huawei2020)
SET(CMAKE_BUILD_TYPE "Debug")
#SET(CMAKE_CXX_FLAGS_DEBUG "-g -Wall") 
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(
    Test
    "${SOURCE_DIR}/main.cpp"
)

target_link_libraries(Test pthread)