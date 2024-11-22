cmake_minimum_required(VERSION 3.28)

project(FourierSeries)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # generate compiler_commands.json for clangd

find_package(SDL2 REQUIRED)

set(
  IMGUI_DIRS
  ${PROJECT_SOURCE_DIR}/build/vendor/imgui
)

set(
  IMGUI_BACKEND_DIRS
  ${PROJECT_SOURCE_DIR}/build/vendor/imgui/backends
)

include_directories(
  ${PROJECT_NAME}
  ${PROJECT_SOURCE_DIR}/include
  ${IMGUI_DIRS}
  ${IMGUI_BACKEND_DIRS}
  ${SDL2_INCLUDE_DIRS}
)

set(
  SOURCES
  ${PROJECT_SOURCE_DIR}/src/main.cpp
  ${PROJECT_SOURCE_DIR}/include/FourierSeries.cpp
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

# put executable in project root
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(
  ${PROJECT_NAME}
  ${SDL2_LIBRARIES}
)
