find_package(Catch2 2.0 REQUIRED)

add_executable(test_cmast
  test_main.cpp
  parser/test.cpp
  parser/test_lexer.comments.cpp
  parser/test_lexer.identifier.cpp
)

target_include_directories(test_cmast
  PRIVATE
    $<TARGET_PROPERTY:cmast,INCLUDE_DIRECTORIES>
)

target_link_libraries(test_cmast
  PRIVATE
    CMAST::cmast
    Catch2::Catch
)

add_test(unit_tests test_cmast)
