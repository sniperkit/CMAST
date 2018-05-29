set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)

if(_TOOLCHAIN_SETUP)
  return()
endif()

set(_TOOLCHAIN_SETUP ON)

if(EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
  message(STATUS "Setting up Conan packages")
  include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
  conan_define_targets()
  conan_set_find_paths()
else()
  set(_TOOLCHAIN_SETUP OFF)
endif()
