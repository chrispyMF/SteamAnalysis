cmake_minimum_required(VERSION 3.20)
project(steamanalysis_backend LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Drogon CONFIG REQUIRED)
find_package(CURL REQUIRED)

add_executable(steamanalysis
    src/main.cpp
    src/test.cpp
    src/clients/SteamWebApiClient.cpp
    src/helper.cpp
    src/logging/logger.cpp
)

add_library(nlohmann INTERFACE)

target_include_directories(steamanalysis INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

target_link_libraries(steamanalysis PRIVATE 
  nlohmann 
  Drogon::Drogon
  CURL::libcurl
)

target_include_directories(steamanalysis PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)
