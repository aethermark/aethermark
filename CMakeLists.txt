cmake_minimum_required(VERSION 3.15...3.27)

project(
  ${SKBUILD_PROJECT_NAME}
  VERSION ${SKBUILD_PROJECT_VERSION}
  LANGUAGES CXX)

find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)
find_package(pybind11 REQUIRED)

# === Collect source files ===

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# === Add Python module ===

python_add_library(_core MODULE ${SOURCE_FILES} WITH_SOABI)

target_link_libraries(_core PRIVATE pybind11::headers)

target_include_directories(_core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(_core PRIVATE cxx_std_17)
set_target_properties(_core PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED
                                                       YES)

target_compile_definitions(_core PRIVATE VERSION_INFO=${PROJECT_VERSION})

install(TARGETS _core DESTINATION aethermark)
