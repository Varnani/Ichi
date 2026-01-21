cmake_minimum_required(VERSION 3.12.0)
project(stb VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(stb STATIC)

target_sources(stb PRIVATE "stb_image_impl.cpp")
target_include_directories(stb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})