cmake_minimum_required(VERSION 3.16.0)
project(Ichi VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(${CMAKE_PROJECT_NAME} WIN32 MACOSX_BUNDLE)
target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_20)

# simd option
option(USE_SIMD "Use SIMD intrinsics" ON)
if(USE_SIMD)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE SIMD_ON)
    if (APPLE)
        target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE NEON)
    else()
        target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE AVX)
    endif()
endif()

# multithreading option
option(USE_MULTITHREADING "Enable multithreaded codepath" ON)
if(USE_MULTITHREADING)
    if(APPLE)
        message(WARNING "Can't use multithreading on Apple.")
    else()
        target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE MULTITHREADING_ON)
    endif()
endif()

# whole-program optimizations
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)
if(ipo_supported)
    set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
endif()

# compiler arguments
if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    # output debug info even with release builds
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /Zi)
    target_link_options(${CMAKE_PROJECT_NAME} PRIVATE /DEBUG)

    if(USE_SIMD)
            target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /arch:AVX2)
    endif()

else()
    # output debug info even with release builds
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -g)

    if(USE_SIMD)
        if(NOT APPLE)
            target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -mavx2)
        endif()
    endif()
endif()

# glob up project sources
file(GLOB ichi_sources CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/ichi/source/*.cpp)
target_sources(${CMAKE_PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/ichi/Main.cpp")
target_sources(${CMAKE_PROJECT_NAME} PUBLIC ${ichi_sources})

# add "copy resources" command as build dependency
add_custom_target(copy_resources COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/copy_resources.in)
add_dependencies(${CMAKE_PROJECT_NAME} copy_resources)

# platform defines
if(WIN32)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE PLATFORM_WINDOWS)
elseif(APPLE)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE PLATFORM_APPLE)
elseif(UNIX)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE PLATFORM_UNIX)
endif()

# rgfw
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/rgfw/)
target_link_libraries(${CMAKE_PROJECT_NAME} rgfw)

# glm
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/glm/)
target_link_libraries(${CMAKE_PROJECT_NAME} glm)

# stb
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/stb/)
target_link_libraries(${CMAKE_PROJECT_NAME} stb)

# macos bundle properties
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.varnani.${CMAKE_PROJECT_NAME}
    MACOSX_BUNDLE_BUNDLE_NAME ${CMAKE_PROJECT_NAME}
    MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/template_info.plist.in
)
