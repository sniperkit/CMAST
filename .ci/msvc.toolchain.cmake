set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)

add_compile_options(
  /W4
  # Warnings as error for CI builds
  /Wx
)

include("C:/tools/vcpkg/scripts/buildsystems/vcpkg.cmake")
