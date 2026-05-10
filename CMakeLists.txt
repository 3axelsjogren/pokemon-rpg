cmake_minimum_required(VERSION 3.16)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
project(PokemonRPG)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find raylib (installed via apt or built from source)
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    # Fallback: fetch raylib via FetchContent
    include(FetchContent)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG        5.0
    )
    FetchContent_MakeAvailable(raylib)
endif()

# Source files
file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES}
        src/MapManager.cpp
        src/MapManager.h)

target_link_libraries(${PROJECT_NAME} raylib)

# Linux requires these
if (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} m pthread dl)
endif()

# Copy assets next to the binary
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)
