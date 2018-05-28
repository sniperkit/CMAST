#[[
This file is part of CMAST

Copyright (C) 2018  Justin Bassett

CMAST is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

CMAST is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
]]

cmake_minimum_required(VERSION 3.8)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

###################
# CMAST Definition
##
project(CMAST VERSION 0.1.0 LANGUAGES CXX)

include(CTest)
include(GetCMakeLib)

option(CMAST_BUILD_TESTING "Build Testing option for CMAST. Defaults to BUILD_TESTING" "${BUILD_TESTING}")

find_package(Boost 1.67.0 REQUIRED)

add_library(CMAST
  src/cmast.cpp

  src/ast/FunctionArgument.cpp
  src/ast/FunctionCall.cpp
  src/ast/ListFile.cpp
  src/ast/SourceInfo.cpp

  src/ast/level1/argument.cpp
  src/ast/level1/bracket_argument.cpp
  src/ast/level1/command_invocation.cpp
  src/ast/level1/comment.cpp
  src/ast/level1/file.cpp
  src/ast/level1/file_element.cpp
  src/ast/level1/identifier.cpp
  src/ast/level1/identifier.parse.cpp
  src/ast/level1/quoted_argument.cpp
  src/ast/level1/unquoted_argument.cpp
)

add_library(CMAST::CMAST ALIAS CMAST)

target_include_directories(CMAST
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# Note: to build with libstdc++, you also have to add -lstdc++fs to the link commands in your toolchain file
target_link_libraries(CMAST
  PUBLIC
    Boost::boost
)

target_compile_features(CMAST
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
  TARGETS CMAST
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
