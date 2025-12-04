# uninstall.cmake
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()

message(STATUS "Uninstalling Aethermark library and headers from ${CMAKE_INSTALL_PREFIX}")

# Remove installed library
file(REMOVE "${CMAKE_INSTALL_PREFIX}/lib/libaethermark.a")

# Remove installed headers
file(REMOVE_RECURSE "${CMAKE_INSTALL_PREFIX}/include/aethermark")
