cmake_minimum_required(VERSION 3.5)

project(airduct LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(airduct_VERSION "0.2.0")

include("${CMAKE_SOURCE_DIR}/vendor/rapidjson.cmake")

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}
    ${GFLAGS_INCLUDE_DIR}
    ${BENCHMARK_INCLUDE_DIR}
    ${RAPIDJSON_INCLUDE_DIR}
)

include("${CMAKE_SOURCE_DIR}/src/local.cmake")
