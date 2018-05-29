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
  return()
endif()

macro(find_package NAME)
  string(TOLOWER "${NAME}" _NAME_LOWER)

  if("${_NAME_LOWER}" STREQUAL "catch2")
    add_library(Catch2::Catch INTERFACE IMPORTED)

    set(_properties
      INTERFACE_LINK_LIBRARIES
      INTERFACE_INCLUDE_DIRECTORIES
      INTERFACE_COMPILE_DEFINITIONS
      INTERFACE_COMPILE_OPTIONS
    )

    foreach(_property ${_properties})
      get_target_property(_prop_value CONAN_PKG::catch2 ${_property})
      set_target_properties(Catch2::Catch
        PROPERTIES
          ${_property} "${_prop_value}"
      )
    endforeach()
  else()
    _find_package(${ARGV})
  endif()
endmacro()
