# oomuse-core

Small core C++ library with essentials that all other oomuse libraries are built on top of. This library is not music-related, so feel free to use it in any C++ project.


## What's in the Core Library?

File or Class | Description
--------------|--------------------------------------------------
[int_types](https://github.com/Lindurion/oomuse-core/blob/master/include/oomuse/core/int_types.h) | Shorter to type aliases like `int32` and `uint64`
[readability_macros](https://github.com/Lindurion/oomuse-core/blob/master/include/oomuse/core/readability_macros.h) | `CANT_COPY(MyClass)`, `CANT_MOVE(MyClass)`, `CALL_MEMBER_FN()`, ...
[constexpr_assert](https://github.com/Lindurion/oomuse-core/blob/master/include/oomuse/core/constexpr_assert.h) | `CONSTEXPR_ASSERT()`, for asserts in constexpr functions
[FixedArray](https://github.com/Lindurion/oomuse-core/blob/master/include/oomuse/core/FixedArray.h) | Runtime-determined fixed-length array
[Optional](https://github.com/Lindurion/oomuse-core/blob/master/include/oomuse/core/Optional.h) | Optional value that may or may not be present (useful return type)
[Validator](https://github.com/Lindurion/oomuse-core/blob/master/include/oomuse/core/Validator.h), <br> [Validators](https://github.com/Lindurion/oomuse-core/blob/master/include/oomuse/core/Validators.h) | Simple value validation for numeric ranges, non-empty strings, etc.
[strings](https://github.com/Lindurion/oomuse-core/blob/master/include/oomuse/core/strings.h) | Simple string utilities like case conversion and whitespace trimming


## Prerequisites

- A modern C++ compiler toolchain that supports most C++14 language features and libraries. Recommendations:
  - **Windows**: [Microsoft Visual Studio Community 2015](https://www.visualstudio.com/vs/community/) (make sure to choose custom installation and install support for C++).
  - **Mac**: [Xcode](https://developer.apple.com/xcode/download/), which should install Clang (otherwise see [Xcode Command Line Tools](https://developer.apple.com/library/ios/technotes/tn2339)); or you can directly install the [Clang](http://llvm.org/releases/download.html) compiler.
  - **Linux**: [GCC](https://gcc.gnu.org/) (almost certainly preinstalled) or [Clang](http://llvm.org/releases/download.html).
- A recent version (3.0 or higher) of [CMake](https://cmake.org/download/).
- A recent version of [conan](https://conan.io/downloads), which is used a C++ package manager that downloads and builds needed dependencies.


## Using oomuse-core in your own projects

You can use [any build system that conan supports](http://conanio.readthedocs.io/en/latest/integrations.html), though **CMake** is highly recommended (since it is the most widely supported and gives you the flexibility to build your project from most popular compilers and IDEs, including Xcode and Visual Studio).

Add a requires line for **oomuse-core** within your `conanfile.txt` file (shown using CMake here):
```
[requires]
oomuse-core/0.1.2@lindurion/stable

[generators]
cmake
```

Or, for more advanced projects, you can list this dependency within a `conanfile.py` file instead (see [docs](http://docs.conan.io/en/latest/conanfile_py.html)).

If your project is using CMake, include the (about to be) generated `conanbuildinfo.cmake` file that configures all your conan dependencies (including oomuse-core), setup conan (which makes sure, for example, that all needed header files are now on your include path), and then link your binary or library against all conan-downloaded libs (which will include oomuse-core). For example, within your `CMakeLists.txt`:

```
cmake_minimum_required(VERSION 3.0.2)
project(YourProject)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(your_project your_project.cpp)
target_link_libraries(your_project ${CONAN_LIBS})
```

Include and use any **oomuse-core** code from your own. For example, from `your_project.cpp`:

```c++
#include "oomuse/core/strings.h"

namespace strings = oomuse::strings;

int main() {
  // …
  doSomethingWith(strings::trimWhitespace(someUserInput));
  // …
}
```

Finally, install conan dependencies (including oomuse-core) and run your build from the command line:
```
$ cd /path/to/yourproject/build
$ conan install /path/to/yourproject/src --build=missing

$ cmake /path/to/yourproject/src -G "Visual Studio 14 2015 Win64"  # Or "Xcode", "Unix Makefiles"
$ cmake --build . --config Release
$ ./bin/your_project
```


## Developing oomuse-core & Running Tests

To contribute changes to this library (*vs.* just using it), you should first clone it on GitHub.

Recommend building from a separate directory (that's not underneath the cloned git project, known as an out-of-source build).

Build & run tests:
```
$ cd /path/to/build/oomuse-core/
$ conan install /path/to/cloned/src/for/oomuse-core -o testing=True -s build_type=Debug --build=missing
$ conan build /path/to/cloned/src/for/oomuse-core
```


## License

This open source library is free to use in your own projects, released under the [Apache License Version 2.0](https://github.com/Lindurion/oomuse-core/blob/master/LICENSE).
