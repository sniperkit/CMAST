cmake_minimum_required(VERSION 3.8)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

###################
# CMAST Definition
##
project(cmast VERSION 0.1.0 LANGUAGES CXX)

include(CTest)
include(GetCMakeLib)

option(CMAST_BUILD_TESTING "Build Testing option for cmast. Defaults to BUILD_TESTING" "${BUILD_TESTING}")

find_package(Boost 1.67.0 REQUIRED)

get_cmake(CMakeLib)

add_library(cmast
  src/cmast.cpp
  src/parser/lexer.cpp
  src/parser/lexer/comments.cpp
  src/parser/lexer/identifiers.cpp
)

add_library(CMAST::cmast ALIAS cmast)

target_include_directories(cmast
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(cmast
  PUBLIC
    Boost::boost
  PRIVATE
    CMakeLib
)

target_compile_features(cmast
  PUBLIC cxx_std_17
)

##################
# Automated Tests
##

if(CMAST_BUILD_TESTING)
  add_subdirectory(test)
endif()

###############
# Installation
##

include(GNUInstallDirs)
set(CMAST_INSTALL_CONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/CMAST")

install(
  TARGETS cmast
  EXPORT CMASTTargets
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)

install(
  EXPORT CMASTTargets
  FILE CMASTTargets.cmake
  NAMESPACE CMAST::
  DESTINATION "${CMAST_INSTALL_CONFIGDIR}"
)

install(
  DIRECTORY include/
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/CMASTConfigVersion.cmake"
  COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
  "${CMAKE_CURRENT_LIST_DIR}/cmake/CMASTConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/CMASTConfig.cmake"
  INSTALL_DESTINATION "${CMAST_INSTALL_CONFIGDIR}"
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/CMASTConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/CMASTConfigVersion.cmake"
  DESTINATION "${CMAST_INSTALL_CONFIGDIR}"
)
