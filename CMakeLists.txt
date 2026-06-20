cmake_minimum_required(VERSION 3.10)

project(McLearningLab)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.c" "${CMAKE_SOURCE_DIR}/src/core/*.cpp" "${CMAKE_SOURCE_DIR}/src/render/*.cpp" "${CMAKE_SOURCE_DIR}/src/world/*.cpp" ${CMAKE_SOURCE_DIR}/src/gui/*.cpp ${CMAKE_SOURCE_DIR}/src/player/*.cpp ${CMAKE_SOURCE_DIR}/src/item/*.cpp ${CMAKE_SOURCE_DIR}/src/util/*.cpp ${CMAKE_SOURCE_DIR}/src/config/*.cpp)

add_executable(McLearningLab ${SRC_FILES})

set_target_properties(McLearningLab PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

target_include_directories(McLearningLab PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src/core ${CMAKE_SOURCE_DIR}/src/render ${CMAKE_SOURCE_DIR}/src/world ${CMAKE_SOURCE_DIR}/src/gui ${CMAKE_SOURCE_DIR}/src/player ${CMAKE_SOURCE_DIR}/src/item ${CMAKE_SOURCE_DIR}/src/util ${CMAKE_SOURCE_DIR}/src/config)
target_link_directories(McLearningLab PRIVATE ${CMAKE_SOURCE_DIR}/lib ${CMAKE_SOURCE_DIR}/bin)
target_link_libraries(McLearningLab PRIVATE glfw3 opengl32)