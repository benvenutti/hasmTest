# hasm [![Build Status](https://travis-ci.org/benvenutti/hasm.svg?branch=master)](https://travis-ci.org/benvenutti/hasm) [![Build status](https://ci.appveyor.com/api/projects/status/xvvgrlygu5hofm75?svg=true)](https://ci.appveyor.com/project/benvenutti/hasmtest) <a href="https://scan.coverity.com/projects/benvenutti-hasm"><img alt="Coverity Scan Build Status" src="https://scan.coverity.com/projects/9220/badge.svg"/></a> [![Coverage Status](https://coveralls.io/repos/github/benvenutti/hasm/badge.svg?branch=master)](https://coveralls.io/github/benvenutti/hasm?branch=master)

**hasm** stands for Hack Assembler, an assembler for the Hack platform. This project is based on the sixth chapter of the book "The Elements of Computing System: Building a Modern Computer from First Principles", by Nisan and Schocken, where the Hack platform is fully described. For more information, see [nand2tetris](http://www.nand2tetris.org/).

## Overview

The **hasm** assembler is a command line executable. From a valid *.asm* file, **hasm** will generate a valid *.hack* textual binary file. This output file can then be executed in any implementation of the Hack platform.

#### Example 1: assembling a file

Let us take it that there is a file named *add.asm* with the following Hack assembly machine language program:

```sh
@2
D=A
@3
D=D+A
@0
M=D
```

We can run the assembler from the command line, passing the path for the source file as an argument:

```sh
$ ./hasm add.asm
```

This will generate an output file called *add.hack*, with the following content:

```sh
0000000000000010
1110110000010000
0000000000000011
1110000010010000
0000000000000000
1110001100001000
```

This file can then be loaded into any Hack-compliance machine and it will correctly reflect the logic of the program described in the source file.

#### Example 2: getting help

Running **hasm** in the command line with the argument *--help* (or the shorthand *-h*) will print the usage message:

```
$ ./hasm --help
Usage: hasm [options] <input file>.asm
Allowed options:
  -s [ --symbol-table ]   export symbol table (to <input file>.sym)
  -i [ --input-file ] arg input .asm file
  -h [ --help ]           print this help message
```

## Compiling **hasm** from source

The assembler is written in C++11 and uses [CMake](https://cmake.org/) to manage the building process. Aside from the C++ Standard Library, **hasm** uses [Boost](http://www.boost.org/).

The following list enumerates the dependencies minimum requirements:

* cmake 2.8
* libboost 1.54.0

### Supported compilers

The following compilers are known to work for **hasm**:

* GCC 4.8 - 6.0
* Clang 3.5
* Microsoft Visual C++ 2014 Build Tools 14.0.25420.1

### Cross-platform

One of the goals of **hasm** is to be a cross-platform project. The following table presents the operating systems and the compilers used in continuous integration at [Travis](https://travis-ci.org/benvenutti/hasm/) and [AppVeyor](https://ci.appveyor.com/project/benvenutti/hasmtest):

| Compiler        | Operating System             | Version String |
|-----------------|------------------------------|----------------|
| GCC 4.8.5       | Ubuntu 14.04.3 LTS           | g++-4.8 (Ubuntu 4.8.5-2ubuntu1~14.04.1) 4.8.5 |
| GCC 4.9.3       | Ubuntu 14.04.3 LTS           | g++-4.9 (Ubuntu 4.9.3-8ubuntu2~14.04) 4.9.3 |
| GCC 5.3.0       | Ubuntu 14.04.3 LTS           | g++-5 (Ubuntu 5.3.0-3ubuntu1~14.04) 5.3.0 20151204 |
| GCC 6.1.1       | Ubuntu 14.04.3 LTS           | g++-6 (Ubuntu 6.1.1-3ubuntu11~14.04.1) 6.1.1 20160511 |
| Clang 3.5.0     | Ubuntu 14.04.3 LTS           | clang version 3.5.0 (tags/RELEASE_350/final) |
| Clang Xcode 7.3.1 | Darwin Kernel Version 15.6.0 (OS X 10.11)           | Apple LLVM version 7.3.0 (clang-703.0.31) |
| Clang Xcode 8.0 beta 4 | Darwin Kernel Version 15.6.0 (OS X 10.11)           | Apple LLVM version 8.0.0 (clang-800.0.35) |
| Visual Studio 14 2015 | Windows Server 2012 R2 (x64)          | Microsoft (R) Build Engine version 14.0.25420.1 |

### Compiling

Run the classic *cmake + make* on the source directory. It is recommend to run *cmake* from out of source, that is, usually from a *build* directory inside the source directory.

```shh
hasm$ mkdir build
hasm$ cd build
hasm$ cmake ..
hasm$ make
```

### Running tests

To run tests, you need to execute the *test* target using as so:

```shh
hasm$ make test
...
100% tests passed, 0 tests failed out of 7
```
