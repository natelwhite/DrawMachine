# set(IMGUI_DIRS ${PROJECT_SOURCE_DIR}/vendor/imgui)
# set(IMGUI_BACKEND_DIRS ${IMGUI_DIRS}/backends)

file(
  GLOB SOURCES
  main.cpp
  App.cpp
  Machine.cpp
  # ${IMGUI_DIRS}/*.cpp
  # ${IMGUI_BACKEND_DIRS}/*.cpp
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${SOURCES})
