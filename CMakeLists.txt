cmake_minimum_required(VERSION 3.28)

set(CMAKE_BUILD_TYPE Debug)

project(DrawMachine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # generate compiler_commands.json for clangd

add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(vendor)
add_subdirectory(src)
add_subdirectory(include)

target_link_libraries(${PROJECT_NAME} PRIVATE vendor)
