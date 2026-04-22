cmake_minimum_required(VERSION 3.20)
project(steamanalysis_backend LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Drogon CONFIG REQUIRED)
find_package(CURL REQUIRED)

add_executable(steamanalysis
    src/Main.cpp
    src/utils/Test.cpp
    src/utils/Helper.cpp
    src/logging/Logger.cpp
    src/clients/HttpClient.cpp
    src/clients/SteamWebApiClient.cpp
    src/models/SteamPlayer.cpp
    src/services/SteamPlayerService.cpp 
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
