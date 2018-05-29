set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)

if(NOT _TOOLCHAIN_SETUP)
  set(_TOOLCHAIN_SETUP ON)

  macro(find_package NAME)
    if(NOT _TOOLCHAIN_CONAN_SETUP)
      set(_TOOLCHAIN_CONAN_SETUP ON)
      include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
      conan_set_find_paths()
    endif()

    _find_package(${ARGV})
  endmacro()
endif()
