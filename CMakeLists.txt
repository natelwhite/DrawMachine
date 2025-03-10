cmake_minimum_required(VERSION 3.28)
project(DrawMachine)
add_executable(${PROJECT_NAME} ${SOURCES})
add_subdirectory(vendor)
add_subdirectory(src)
add_subdirectory(include)
target_link_libraries(${PROJECT_NAME} PRIVATE vendor)

# copy data file to executable location
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/source.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
