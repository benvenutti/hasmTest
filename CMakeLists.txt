cmake_minimum_required( VERSION 3.6 FATAL_ERROR )
project( hasm VERSION 0.2.1 LANGUAGES CXX )

# policies:

if ( POLICY CMP0074 )
  cmake_policy( SET CMP0074 NEW )
endif()

# build settings:

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release FORCE )
endif()

if( CMAKE_BUILD_TYPE MATCHES Coverage )
  add_compile_options( -g -O0 --coverage )
  set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov --coverage" )
  set( CMAKE_BUILD_TYPE Debug FORCE )
endif()

set( CMAKE_CXX_STANDARD 14 )
set( CXX_STANDARD_REQUIRED ON )
set( CXX_EXTENSIONS OFF )
set( CMAKE_CXX_FLAGS_RELEASE -O3 )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

# 3rd-party dependencies:

find_package( Boost
  COMPONENTS
    filesystem
    system
    REQUIRED )

add_library( catch2 INTERFACE )
target_include_directories( catch2 INTERFACE libs/Catch2/single_include )

add_library( clara INTERFACE )
target_include_directories( clara INTERFACE libs/Clara/single_include )

# compiler definitions:

if( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
  add_compile_options( /W4 /w14545 /w34242 /w34254 /w34287 /w44263 /w44265 
    /w44296 /w44311 /w44826 /we4289 /w14546 /w14547 /w14549 /w14555 /w14619 
    /w14905 /w14906 /w14928 )
elseif( CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU" )
  add_compile_options( -Wall -Wextra -Wconversion -Wsign-conversion -Wshadow
    -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wcast-qual -Wunused
    -Woverloaded-virtual -pedantic )
endif()

# hack_assembler library:

set( hack_assembler_source_files
  src/Assembler.cpp
  src/AssemblerEngine.cpp
  src/Coder.cpp
  src/CommandLineParser.cpp
  src/ErrorMessage.cpp
  src/FileHandler.cpp
  src/HackCommandParser.cpp
  src/Parser.cpp
  src/SymbolTable.cpp
  src/SymbolTableWriter.cpp )

set( hack_assembler_header_files
  include/Assembler.hpp
  include/AssemblerEngine.hpp
  include/Coder.hpp
  include/CommandLineParser.hpp
  include/Config.hpp
  include/ErrorMessage.hpp
  include/FileHandler.hpp
  include/Hack.hpp
  include/HackCommandParser.hpp
  include/HackGrammar.hpp
  include/HackLex.hpp
  include/Hasm.hpp
  include/Parser.hpp
  include/SymbolTable.hpp
  include/SymbolTableWriter.hpp )

configure_file(
  ${PROJECT_SOURCE_DIR}/include/Version.hpp.in
  ${PROJECT_BINARY_DIR}/include/Version.hpp )

add_library( hack_assembler 
  ${hack_assembler_source_files} 
  ${hack_assembler_header_files} )

target_include_directories( hack_assembler
  PUBLIC
    include
    ${PROJECT_BINARY_DIR}/include )

target_link_libraries( hack_assembler
  PUBLIC
    Boost::boost
  PRIVATE
    Boost::filesystem
    Boost::system
    clara )

# hasm executable:

add_executable( hasm src/main.cpp )

target_link_libraries( hasm
  PRIVATE
    hack_assembler )

# unit tests:

enable_testing()
include( CTest )

if( BUILD_TESTING )
  add_subdirectory( tests )
endif()
