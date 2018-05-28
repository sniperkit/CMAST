# Using this library

CMAST is built using CMake. Build and install CMAST, then use

```cmake
find_package(CMAST 0.1.0)

# ...

target_link_libraries(MyTarget
  CMAST::CMAST
)
```

## Building instructions

CMAST depends on Boost. Ensure you have Boost installed.

## Building with Conan

conanfile.txt (catch2 is only required for building tests):

```
[requires]
boost/1.67.0@conan/stable
catch2/2.2.2@bincrafters/stable

[generators]
cmake

[options]
boost:header_only = True
```

Define a toolchain.cmake:

```cmake
# Set the appropriate settings for these variables
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER <compiler>)
set(CMAKE_C_COMPILER <compiler>)
set(CMAKE_BUILD_TYPE <Debug|Release|RelWithDebInfo>)

# To build with warnings:
add_compile_options(
  -Wall
  -Wextra
)

# Required for libstdc++'s std::filesystem library
link_libraries(stdc++fs)

# To allow `find_package` to use Conan's packages
macro(find_package NAME)
  if(NOT _TOOLCHAIN_CONAN_SETUP)
    set(_TOOLCHAIN_CONAN_SETUP ON)
    include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    conan_define_targets()
    conan_set_find_paths()
  endif()

  _find_package(${ARGV})
endmacro()
```

After running `conan install <path to conanfile.txt>`, run

```
$ pwd
/home/user/cmast/build
$ cmake -G <generator> -DCMAKE_TOOLCHAIN_FILE=<path to toolchain> ..
```
