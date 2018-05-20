

function(_execute_cmake)
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
      # BUGGG: BUILD_TESTING OFF. OFF is considered false!
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

function(setup_cmake_external_project_file WORKING_DIRECTORY EP_SOURCE_DIR_FILE)
  # Note: EP_SOURCE_DIR_FILE is referenced in ep.cmake.in
  configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/cmake/GetCMake/ep.cmake.in"
    "${WORKING_DIRECTORY}/CMakeLists.txt"
    @ONLY)
endfunction()

function(download_cmake WORKING_DIRECTORY)
  _execute_cmake(
    RESULT_VARIABLE download-failed
    WORKING_DIRECTORY "${WORKING_DIRECTORY}"
  )

  if(download-failed)
    message(FATAL_ERROR "Failed to download CMake")
  endif()
endfunction()

function(build_cmake TARGET_FILE_OUT WORKING_DIRECTORY EP_SOURCE_DIR BUILD_DIR)
  set(CMAKE_SUBDIRECTORY "${EP_SOURCE_DIR}")
  set(TARGET_FILE "${WORKING_DIRECTORY}/target-file")

  configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/cmake/GetCMake/GetTargetFile.cmake.in"
    "${WORKING_DIRECTORY}/get-target-file/CMakeLists.txt"
    @ONLY)

  _execute_cmake(
    RESULT_VARIABLE get-target-file-failed
    WORKING_DIRECTORY "${WORKING_DIRECTORY}/get-target-file"
  )

  if(get-target-file-failed)
    message(FATAL_ERROR "Unable to configure CMakeLib")
  endif()

  file(READ "${TARGET_FILE}" RESULT)
  set(${TARGET_FILE_OUT} "${RESULT}" PARENT_SCOPE)

  execute_process(
    COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE build-failed
    WORKING_DIRECTORY "${WORKING_DIRECTORY}/get-target-file"
  )

  if(build-failed)
    message(FATAL_ERROR "Failed to build CMake")
  endif()
endfunction()

function(make_cmakelib_imported_target TARGET_NAME EP_SOURCE_DIR BUILD_DIR CMAKELIB_ARCHIVE_FILE)
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
endfunction()

function(get_cmake TARGET_NAME)
  set(WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/get-cmake")
  set(EP_SOURCE_DIR_FILE "${WORKING_DIRECTORY}/ep-source-dir")

  setup_cmake_external_project_file("${WORKING_DIRECTORY}" "${EP_SOURCE_DIR_FILE}")
  download_cmake("${WORKING_DIRECTORY}")
  file(READ "${EP_SOURCE_DIR_FILE}" EP_SOURCE_DIR)

  set(BUILD_DIR "${WORKING_DIRECTORY}/build-dir")
  build_cmake(CMAKELIB_ARCHIVE_FILE "${WORKING_DIRECTORY}" "${EP_SOURCE_DIR}" "${BUILD_DIR}")

  make_cmakelib_imported_target(${TARGET_NAME} "${EP_SOURCE_DIR}" "${BUILD_DIR}" "${CMAKELIB_ARCHIVE_FILE}")
endfunction()
