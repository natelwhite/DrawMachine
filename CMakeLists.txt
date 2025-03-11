cmake_minimum_required(VERSION 3.28)
project(DrawMachine)
add_executable(${PROJECT_NAME})
add_subdirectory(vendor)
add_subdirectory(src)
add_subdirectory(include)
target_link_libraries(${PROJECT_NAME} PRIVATE vendor)

# copy data files to binary dir
file(
	GLOB DATA
	${CMAKE_CURRENT_SOURCE_DIR}/data/*
)
file(COPY ${DATA} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)
