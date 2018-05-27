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

# Executes cmake's configure step on the current directory `.` (this does an in-source build)
# Certain cmake arguments are settable as parameters. E.g.:
# 
#     gcl_execute_cmake(CMAKE_BUILD_TYPE Debug)
# 
# When not explicitly specified, these variables are set to the current values
# in the currently executing environment (so `CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}`
# is redundant). If you wish to not use the specific argument, pass in the
# NO_CMAKE_BUILD_TYPE argument and it will not be passed in to the cmake executable.
# 
# Note that this is specifically for this file, so BUILD_TESTING and BUILD_SHARED_LIBS
# are both OFF, unsettable by the user.
function(gcl_execute_cmake)
  set(options
    NO_CMAKE_GENERATOR
    NO_CMAKE_BUILD_TYPE
    NO_CMAKE_PREFIX_PATH
    NO_CMAKE_TOOLCHAIN_FILE
    NO_CMAKE_CXX_COMPILER
    NO_CMAKE_C_COMPILER
  )
  set(oneValueArgs
    CMAKE_GENERATOR
    CMAKE_BUILD_TYPE
    CMAKE_PREFIX_PATH
    CMAKE_TOOLCHAIN_FILE
    CMAKE_CXX_COMPILER
    CMAKE_C_COMPILER

    RESULT_VARIABLE
    WORKING_DIRECTORY
  )

  set(multiValueArgs)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  macro(compute_gen_arg NAME)
    if(NOT ARG_NO_${NAME})
      if(DEFINED ARG_${NAME})
        set(CMD_${NAME} "-G${ARG_${NAME}}")
      elseif(DEFINED ${NAME})
        set(CMD_${NAME} "-G${${NAME}}")
      else()
        set(CMD_${NAME})
      endif()
    endif()
  endmacro()

  macro(compute_defs)
    foreach(NAME ${ARGV})
      if(NOT ARG_NO_${NAME})
        if(DEFINED ARG_${NAME})
          set(CMD_${NAME} "-D${NAME}=${ARG_${NAME}}")
        elseif(DEFINED ${NAME})
          set(CMD_${NAME} "-D${NAME}=${${NAME}}")
        else()
          set(CMD_${NAME})
        endif()
      endif()
    endforeach()
  endmacro()

  compute_gen_arg(CMAKE_GENERATOR)
  
  compute_defs(
    CMAKE_BUILD_TYPE
    CMAKE_PREFIX_PATH
    CMAKE_TOOLCHAIN_FILE
    CMAKE_CXX_COMPILER
    CMAKE_C_COMPILER
  )

  execute_process(
    COMMAND ${CMAKE_COMMAND} .
      "${CMD_CMAKE_GENERATOR}"
      "${CMD_CMAKE_BUILD_TYPE}"
      "${CMD_CMAKE_PREFIX_PATH}"
      "${CMD_CMAKE_TOOLCHAIN_FILE}"
      "${CMD_CMAKE_CXX_COMPILER}"
      "${CMD_CMAKE_C_COMPILER}"

      -DBUILD_TESTING=OFF
      -DBUILD_SHARED_LIBS=OFF
    RESULT_VARIABLE result
    WORKING_DIRECTORY "${ARG_WORKING_DIRECTORY}"
  )

  set("${ARG_RESULT_VARIABLE}" "${result}" PARENT_SCOPE)
endfunction()

# Sets up the CMakeLists.txt responsible for downloading CMake
function(setup_cmake_external_project_file WORKING_DIRECTORY EP_SOURCE_DIR_FILE)
  # Note: EP_SOURCE_DIR_FILE is referenced in ep.cmake.in
  configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/cmake/GetCMake/ep.cmake.in"
    "${WORKING_DIRECTORY}/CMakeLists.txt"
    @ONLY)
endfunction()

# Actually downloads CMake
function(download_cmake WORKING_DIRECTORY)
  gcl_execute_cmake(
    RESULT_VARIABLE download-failed
    WORKING_DIRECTORY "${WORKING_DIRECTORY}"
  )

  execute_process(
    COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE download-failed2
    WORKING_DIRECTORY "${WORKING_DIRECTORY}"
  )

  if(download-failed OR download-failed2)
    message(FATAL_ERROR "Failed to download CMake")
  endif()
endfunction()

# Builds CMake
# returns the path of libCMakeLib.a in TARGET_FILE_OUT
# 
# Certain extra files are created under ${WORKING_DIRECTORY}:
# * target-file (for computing the result of TARGET_FILE_OUT)
# * target-file-extra (extra dependencies on targets)
# * target-file-extra-names (the names of said dependencies)
# * target-file-extra-nontargets (link dependencies which are not targets)
function(build_cmake TARGET_FILE_OUT WORKING_DIRECTORY EP_SOURCE_DIR BUILD_DIR)
  set(CMAKE_SUBDIRECTORY "${EP_SOURCE_DIR}")
  set(TARGET_FILE "${WORKING_DIRECTORY}/target-file")

  configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/cmake/GetCMake/GetTargetFile.cmake.in"
    "${WORKING_DIRECTORY}/get-target-file/CMakeLists.txt"
    @ONLY)

  gcl_execute_cmake(
    RESULT_VARIABLE get-target-file-failed
    WORKING_DIRECTORY "${WORKING_DIRECTORY}/get-target-file"
  )

  if(get-target-file-failed)
    message(FATAL_ERROR "Unable to configure CMakeLib")
  endif()

  file(READ "${TARGET_FILE}" RESULT)
  set(${TARGET_FILE_OUT} "${RESULT}" PARENT_SCOPE)

  execute_process(
    COMMAND ${CMAKE_COMMAND} --build . --target CMakeLib
    RESULT_VARIABLE build-failed
    WORKING_DIRECTORY "${WORKING_DIRECTORY}/get-target-file"
  )

  if(build-failed)
    message(FATAL_ERROR "Failed to build CMake")
  endif()
endfunction()

# Adds dependencies to target link dependencies
function(cmakelib_imported_target_deps_targets TARGET_NAME TARGET_FILE_EXTRA)
  file(STRINGS "${TARGET_FILE_EXTRA}" dependencies)
  file(STRINGS "${TARGET_FILE_EXTRA}-names" names)

  list(LENGTH names length)
  math(EXPR length "${length} - 1")

  foreach(index RANGE ${length})
    list(GET names ${index} name)
    list(GET dependencies ${index} dep)

    string(STRIP "${name}" name)
    string(STRIP "${dep}" dep)

    add_library(CMakeLib_${name} STATIC IMPORTED)
    set_target_properties(CMakeLib_${name}
      PROPERTIES
        IMPORTED_LOCATION "${dep}"
    )
    
    target_link_libraries(${TARGET_NAME}
      INTERFACE CMakeLib_${name}
    )
  endforeach()
endfunction()

# Adds dependencies to non-target link dependencies
function(cmakelib_imported_target_deps_nontargets TARGET_NAME TARGET_FILE_EXTRA)
  file(STRINGS "${TARGET_FILE_EXTRA}-nontargets" nontargets)

  target_link_libraries(${TARGET_NAME}
    INTERFACE ${nontargets}
  )
endfunction()

# Adds the link dependencies of CMakeLib
function(cmakelib_imported_target_deps TARGET_NAME TARGET_FILE_EXTRA)
  cmakelib_imported_target_deps_targets(${TARGET_NAME} "${TARGET_FILE_EXTRA}")
  cmakelib_imported_target_deps_nontargets(${TARGET_NAME} "${TARGET_FILE_EXTRA}")
endfunction()

# Creates the CMakeLib target
# Include directories and link dependencies are set up appropriately
function(make_cmakelib_imported_target TARGET_NAME WORKING_DIRECTORY EP_SOURCE_DIR BUILD_DIR CMAKELIB_ARCHIVE_FILE)
  set(TARGET_FILE_EXTRA "${WORKING_DIRECTORY}/target-file-extra")

  add_library(${TARGET_NAME} STATIC IMPORTED)
  target_include_directories(${TARGET_NAME}
    INTERFACE
      "${EP_SOURCE_DIR}/Source"
      "${EP_SOURCE_DIR}/Source/LexerParser"
      "${EP_SOURCE_DIR}/Utilities"
      "${BUILD_DIR}/Source"
      "${BUILD_DIR}/Utilities"
  )
  set_target_properties(${TARGET_NAME}
    PROPERTIES
      IMPORTED_LOCATION "${CMAKELIB_ARCHIVE_FILE}"
  )

  cmakelib_imported_target_deps(${TARGET_NAME} ${TARGET_FILE_EXTRA})
endfunction()

# Obtains CMakeLib
# Downloads CMake, and sets up a target TARGET_NAME which can be used to specify
# a dependency on CMakeLib via target_link_libraries
function(add_cmake_library TARGET_NAME)
  set(WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/get-cmake")
  set(EP_SOURCE_DIR_FILE "${WORKING_DIRECTORY}/ep-source-dir")

  setup_cmake_external_project_file("${WORKING_DIRECTORY}" "${EP_SOURCE_DIR_FILE}")
  download_cmake("${WORKING_DIRECTORY}")
  file(READ "${EP_SOURCE_DIR_FILE}" EP_SOURCE_DIR)

  set(BUILD_DIR "${WORKING_DIRECTORY}/build-dir")
  build_cmake(CMAKELIB_ARCHIVE_FILE "${WORKING_DIRECTORY}" "${EP_SOURCE_DIR}" "${BUILD_DIR}")

  make_cmakelib_imported_target(${TARGET_NAME} "${WORKING_DIRECTORY}" "${EP_SOURCE_DIR}" "${BUILD_DIR}" "${CMAKELIB_ARCHIVE_FILE}")
endfunction()
