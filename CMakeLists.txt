# ---------------------------------------------------------------------------------------
# in-tree build guard:

if( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
  message( FATAL_ERROR "Prevented in-tree build. Please create a build"
      " directory outside of the source code and call cmake from there" )
endif()

# ---------------------------------------------------------------------------------------
# project:

cmake_minimum_required( VERSION 3.8 FATAL_ERROR )
project( hasm VERSION 0.2.1 LANGUAGES CXX )

option( HASM_TESTS "Build hasm tests" ON )

set( CMAKE_CXX_STANDARD 14 )
set( CXX_STANDARD_REQUIRED ON )
set( CXX_EXTENSIONS OFF )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

# ---------------------------------------------------------------------------------------
# policies:

if ( POLICY CMP0074 )
  cmake_policy( SET CMP0074 NEW )
endif()

# ---------------------------------------------------------------------------------------
# build type:

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE )
endif()

if( CMAKE_BUILD_TYPE MATCHES "Coverage" )
  if( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
    message( FATAL_ERROR "Coverage is not for valid compiler ${CMAKE_CXX_COMPILER_ID}" )
  endif()

  set_property( CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug Release RelWithDebInfo MinSizeRel Coverage" )
endif()

# ---------------------------------------------------------------------------------------
# compiler definitions:

if( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
  add_compile_options( /W4 /w14545 /w34242 /w34254 /w34287 /w44263 /w44265
    /w44296 /w44311 /w44826 /we4289 /w14546 /w14547 /w14549 /w14555 /w14619
    /w14905 /w14906 /w14928 )

  # release flags:
  set( CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG" )
endif()

if( CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU" )
  add_compile_options( -Wall -Wextra -Wconversion -Wsign-conversion -Wshadow
    -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wcast-qual -Wunused
    -Woverloaded-virtual -pedantic )

  # release flags:
  set( CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG" )

  # coverage flags:
  set( CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_DEBUG} -O0 --coverage" )

  set( CMAKE_EXE_LINKER_FLAGS_COVERAGE
    "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -lgcov --coverage" )

  set( CMAKE_SHARED_LINKER_FLAGS_COVERAGE "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}" )
  set( CMAKE_STATIC_LINKER_FLAGS_COVERAGE "${CMAKE_STATIC_LINKER_FLAGS_DEBUG}" )
  set( CMAKE_MODULE_LINKER_FLAGS_COVERAGE "${CMAKE_MODULE_LINKER_FLAGS_DEBUG}" )
endif()

# ---------------------------------------------------------------------------------------
# 3rd-party dependencies:

set( Boost_USE_STATIC_LIBS ON )

find_package( Boost
  COMPONENTS
    filesystem
    system
    REQUIRED )

add_library( catch2 INTERFACE )
target_include_directories( catch2 SYSTEM INTERFACE libs/Catch2/single_include )

add_library( clara INTERFACE )
target_include_directories( clara SYSTEM INTERFACE libs/Clara/single_include )

# ---------------------------------------------------------------------------------------
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
    Boost::disable_autolinking
    clara )

# ---------------------------------------------------------------------------------------
# hasm executable:

add_executable( hasm src/main.cpp )

target_link_libraries( hasm
  PRIVATE
    hack_assembler )

# ---------------------------------------------------------------------------------------
# tests:

if( HASM_TESTS )
  enable_testing()
  include( CTest )
  add_subdirectory( tests )
endif()
