<!-- table of contents? -->

# Introduction

CMAST is an [Abstract Syntax Tree (AST)][AST Wikipedia] and parser for CMake
source code. CMAST allows you to create tooling for CMake without having to
worry about writing a parser for it.

CMAST has a few different levels:

* Level 0: The Lexer (currently missing). A fast tokenizer to allow for things
  such as syntax highlighting
* Level 1: A basic AST based very strongly on [CMake's grammar][CMake Syntax]
* Level 2: A higher-level AST, which includes information such as blocks of
  code

  [AST Wikipedia]: https://en.wikipedia.org/wiki/Abstract_syntax_tree
  [CMake Syntax]: https://cmake.org/cmake/help/latest/manual/cmake-language.7.html
