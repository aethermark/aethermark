cmake_minimum_required(VERSION 3.15)
project(aethermark LANGUAGES CXX)

find_package(pybind11 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB AETHERMARK_SOURCES ${CMAKE_SOURCE_DIR}/src/aethermark/*.cpp)

pybind11_add_module(aethermark ${AETHERMARK_SOURCES})

install(TARGETS aethermark LIBRARY DESTINATION aethermark)
