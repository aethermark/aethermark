cmake_minimum_required(VERSION 3.16)
project(aethermark LANGUAGES CXX)

# ==============================
# C++ Standard
# ==============================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ==============================
# Options
# ==============================
option(BUILD_TESTS "Build Google Test unit tests" ON)
option(BUILD_PYTHON "Build Python bindings with pybind11" ON)
option(BUILD_PLAYGROUND "Build playground executable" ON)

# ==============================
# Include directories
# ==============================
include_directories(${PROJECT_SOURCE_DIR}/include)

# ==============================
# Source files
# ==============================
file(GLOB_RECURSE SRC_FILES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

# ==============================
# Build static library
# ==============================
add_library(aethermark STATIC ${SRC_FILES})
target_include_directories(aethermark PUBLIC ${PROJECT_SOURCE_DIR}/include)

# ==============================
# Playground executable
# ==============================
if(BUILD_PLAYGROUND)
    add_executable(playground ${PROJECT_SOURCE_DIR}/playground/main.cpp)
    target_link_libraries(playground PRIVATE aethermark)
    target_include_directories(playground PRIVATE ${PROJECT_SOURCE_DIR}/include)
endif()

# ==============================
# Google Test for C++ tests
# ==============================
if(BUILD_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)
    include(GoogleTest)

    # Collect all test sources
    file(GLOB TEST_SRC "${PROJECT_SOURCE_DIR}/tests/*.cpp")

    # Single executable for all tests
    add_executable(unit_tests ${TEST_SRC})
    target_link_libraries(unit_tests PRIVATE aethermark GTest::GTest GTest::Main pthread)
    target_include_directories(unit_tests PRIVATE ${PROJECT_SOURCE_DIR}/include)

    # Discover all TEST() and TEST_F() cases
    gtest_discover_tests(unit_tests
        DISCOVERY_TIMEOUT 5
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endif()


# ==============================
# Python bindings via pybind11
# ==============================
if(BUILD_PYTHON)
    # Python headers + interpreter
    find_package(Python REQUIRED COMPONENTS Interpreter Development)

    # Pull pybind11 directly from submodule
    add_subdirectory(extern/pybind11)

    # Python bindings source
    file(GLOB_RECURSE PYBIND_SRC "${PROJECT_SOURCE_DIR}/python/bindings/*.cpp")

    # Build Python extension module
    pybind11_add_module(aethermark_py ${PYBIND_SRC})

    # Link with core library
    target_link_libraries(aethermark_py PRIVATE aethermark)

    # Include directory for aethermark headers
    target_include_directories(aethermark_py PRIVATE ${PROJECT_SOURCE_DIR}/include)
endif()

file(GLOB_RECURSE PUBLIC_HEADERS
    "${PROJECT_SOURCE_DIR}/include/aethermark/*.hpp"
    "${PROJECT_SOURCE_DIR}/include/aethermark/*.tpp"
)
set_target_properties(aethermark PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

# ==============================
# Installation
# ==============================
install(
    TARGETS aethermark
    EXPORT aethermarkTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include/aethermark
)
