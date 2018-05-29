set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)

if(_TOOLCHAIN_SETUP)
  return()
endif()

set(_TOOLCHAIN_SETUP ON)

if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/conanbuildinfo.cmake")
  include("${CMAKE_CURRENT_LIST_DIR}/conanbuildinfo.cmake")
  conan_set_find_paths()
endif()
