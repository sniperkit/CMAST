set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)
set(CMAKE_BUILD_TYPE Debug)

add_compile_options(
  -Wall
  -Wextra
  # Warnings as error for CI builds
  -Werror
)

macro(find_package NAME)
  if(NOT _TOOLCHAIN_CONAN_SETUP)
    set(_TOOLCHAIN_CONAN_SETUP ON)
    include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    conan_define_targets()
    conan_set_find_paths()
  endif()

  _find_package(${ARGV})
endmacro()
