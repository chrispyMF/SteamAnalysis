cmake_minimum_required(VERSION 3.20)
project(steamanalysis_backend LANGUAGES CXX)

set(CMAKE_STANDARD 17)
set(CMAKE_STANDARD_REQUIRED ON)

add_executable(steamanalysis
    src/main.cpp
    src/test.cpp
)

add_library(nlohmann INTERFACE)

target_include_directories(steamanalysis INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

target_link_libraries(steamanalysis PRIVATE nlohmann)

target_include_directories(steamanalysis PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)