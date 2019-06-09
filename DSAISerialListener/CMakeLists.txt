cmake_minimum_required(VERSION 2.8)

project(DSAISerialListener)
file(GLOB SOURCES
    src/*.h
    src/*.cpp
)
set(
    CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/build
    )
add_executable(${PROJECT_NAME} ${SOURCES})
find_library(wiringPi_LTB wiringPi)
target_link_libraries(${PROJECT_NAME} ${wiringPi_LTB})

