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

find_package(Catch2 2.0 REQUIRED)

add_executable(TestCMAST
  test_main.cpp

  parser/test.convert_function_argument.cpp
  parser/test.convert_function_call.cpp
  parser/test.convert_source_info.cpp
  parser/test.parse_list_file.cpp
)

target_include_directories(TestCMAST
  PRIVATE
    $<TARGET_PROPERTY:CMAST,INCLUDE_DIRECTORIES>
)

target_link_libraries(TestCMAST
  PRIVATE
    CMAST::CMAST
    Catch2::Catch
)

add_test(TestCMAST TestCMAST)
