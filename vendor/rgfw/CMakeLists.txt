cmake_minimum_required(VERSION 3.12.0)
set(CMAKE_CXX_STANDARD 20)

project(rgfw VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(rgfw STATIC)

target_sources(rgfw PRIVATE "rgfw_impl.cpp")
target_include_directories(rgfw INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(rgfw INTERFACE
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreFoundation"
    "-framework QuartzCore")
