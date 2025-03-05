file(
  GLOB SOURCES
  main.cpp
  App.cpp
  Machine.cpp
)
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${SOURCES})
