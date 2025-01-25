cmake_minimum_required(VERSION 3.28)

set(CMAKE_BUILD_TYPE Debug)

project(DrawMachine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # generate compiler_commands.json for clangd

find_package(SDL2 REQUIRED)

set(
  IMGUI_DIRS
  ${PROJECT_SOURCE_DIR}/vendor/imgui
)

set(
  IMGUI_BACKEND_DIRS
  ${PROJECT_SOURCE_DIR}/vendor/imgui/backends
)

include_directories(
  ${PROJECT_NAME}
  ${PROJECT_SOURCE_DIR}/include
  ${IMGUI_DIRS}
  ${IMGUI_BACKEND_DIRS}
  ${SDL2_INCLUDE_DIRS}
)

file(
  GLOB SOURCES
  ${PROJECT_SOURCE_DIR}/src/main.cpp
  ${PROJECT_SOURCE_DIR}/include/*.cpp
  ${IMGUI_DIRS}/*.cpp
  ${IMGUI_BACKEND_DIRS}/*.cpp
)

set(
  SOURCES
  ${PROJECT_SOURCE_DIR}/src/main.cpp
  ${PROJECT_SOURCE_DIR}/include/Machine.cpp
  ${PROJECT_SOURCE_DIR}/include/App.cpp

  # link imgui
  ${IMGUI_DIRS}/imgui.cpp
  ${IMGUI_DIRS}/imgui_draw.cpp
  ${IMGUI_DIRS}/imgui_demo.cpp
  ${IMGUI_DIRS}/imgui_widgets.cpp
  ${IMGUI_DIRS}/imgui_tables.cpp
  ${IMGUI_BACKEND_DIRS}/imgui_impl_sdl2.cpp
  ${IMGUI_BACKEND_DIRS}/imgui_impl_sdlrenderer2.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(
  ${PROJECT_NAME}
  ${SDL2_LIBRARIES}
)
