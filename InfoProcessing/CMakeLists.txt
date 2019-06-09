cmake_minimum_required(VERSION 2.8)

project(InfoProcessing)
file(GLOB SOURCES
    src/*.hpp
    src/*.cpp
)
set(
    CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/build
    )
add_executable(${PROJECT_NAME} ${SOURCES})

