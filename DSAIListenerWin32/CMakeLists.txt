cmake_minimum_required(VERSION 2.8)

project(DSAIListenerWin32)
file(GLOB SOURCES
    src/*.h
    src/*.cpp
)
set(
    CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/build
    )
add_executable(${PROJECT_NAME} ${SOURCES})

