cmake_minimum_required(VERSION 3.12.0)
project(Ichi VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# glob up project sources
file(GLOB ichi_sources CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/ichi/source/*.cpp
)

add_executable(${CMAKE_PROJECT_NAME} WIN32 MACOSX_BUNDLE)
target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_20)
target_sources(${CMAKE_PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/ichi/Main.cpp")
target_sources(${CMAKE_PROJECT_NAME} PUBLIC ${ichi_sources})

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
