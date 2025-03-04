add_subdirectory(backends)
file(
	GLOB SOURCES
	*.cpp
	backends/*.cpp
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE .)
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${SOURCES})
