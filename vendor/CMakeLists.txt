set(SDL_TEST_LIBRARY OFF)
set(BUILD_SHARED_LIBS ON)
add_subdirectory(SDL)

add_library(vendor INTERFACE)
target_link_libraries(vendor INTERFACE SDL3::SDL3)

# include dear imgui using sdl3 renderer backend
file(
	GLOB SOURCES
	imgui/*.cpp
	imgui/backends/imgui_impl_sdl3.cpp
	imgui/backends/imgui_impl_sdlrenderer3.cpp
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE imgui imgui/backends)
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${SOURCES})
