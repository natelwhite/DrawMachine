cmake_minimum_required(VERSION 3.28)

project(FourierSeries)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # generate compiler_commands.json for clangd

find_package(SDL2 REQUIRED)

include_directories(
  ${PROJECT_NAME}
  ${PROJECT_SOURCE_DIR}/include
  ${SDL2_INCLUDE_DIRS}
)

set(
  SOURCES
  ${PROJECT_SOURCE_DIR}/src/main.cpp
  ${PROJECT_SOURCE_DIR}/include/FourierSeries.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(
  ${PROJECT_NAME}
  ${SDL2_LIBRARIES}
)
