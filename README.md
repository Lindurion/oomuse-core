# oomuse-core

Small core C++ library with essentials that all other oomuse libraries are built on top of. This library is not music-related, so feel free to use it in any C++ project.


## What's in the Core Library?

File or Class | Description
--------------|--------------------------------------------------
[int_types](https://github.com/Lindurion/oomuse-core/blob/master/include/oomuse/core/int_types.h) | Shorter to type aliases like `int32` and `uint64`
[readability_macros](https://github.com/Lindurion/oomuse-core/blob/master/include/oomuse/core/readability_macros.h) | `CANT_COPY(MyClass)`, `CANT_MOVE(MyClass)`, `CALL_MEMBER_FN(obj, fnPtr)`, ...
[FixedArray](https://github.com/Lindurion/oomuse-core/blob/master/include/oomuse/core/FixedArray.h) | Runtime-determined fixed-length array
[Optional](https://github.com/Lindurion/oomuse-core/blob/master/include/oomuse/core/Optional.h) | Optional value that may or may not be present (useful return type)
[Validator](https://github.com/Lindurion/oomuse-core/blob/master/include/oomuse/core/Validator.h), <br> [Validators](https://github.com/Lindurion/oomuse-core/blob/master/include/oomuse/core/Validators.h) | Simple value validation for numeric ranges, non-empty strings, etc.
[strings](https://github.com/Lindurion/oomuse-core/blob/master/include/oomuse/core/strings.h) | Simple string utilities like case conversion and whitespace trimming


## Prerequisites

- A modern C++ compiler toolchain that supports most C++14 language features and libraries. Recommendations:
  - **Windows**: [Microsoft Visual Studio Community 2015](https://www.visualstudio.com/en-us/products/visual-studio-community-vs.aspx) (make sure to choose custom installation and install support for C++).
  - **Mac**: [Xcode](https://developer.apple.com/xcode/download/), which should install Clang (otherwise see [Xcode Command Line Tools](https://developer.apple.com/library/ios/technotes/tn2339)); or you can directly install the [Clang](http://llvm.org/releases/download.html) compiler.
  - **Linux**: [GCC](https://gcc.gnu.org/) (almost certainly preinstalled) or [Clang](http://llvm.org/releases/download.html).
- A recent version (3.0 or higher) of [CMake](https://cmake.org/download/).
- Build and install [Google Test](https://github.com/google/googletest), the C++ unit testing framework used by oomuse-core. See instructions below.


## Install Google Test

One day, a cross-platform build and dependency management tool will exist supporting C++ projects that makes depending on a library as simple as adding a single line and running one build command. But today is not yet that day&mdash;so for now, you've got to jump through a few hoops to get each dependency installed so that CMake projects (this one and your own if you're depending on oomuse-core from another CMake project) can find and use them.

- Clone the [Google Test](https://github.com/google/googletest) git repo somewhere (_e.g._ `C:\github\googletest` or `~/github/googletest`).
- Create a separate directory to use for building Google Test (_e.g._ `C:\cmakebuild\googletest` or `~/cmakebuild/googletest`).
- Set up a CMake build environment and run `cmake-gui` to customize the output location:
  - Change `CMAKE_INSTALL_PREFIX` to a separate install dir (_e.g._ `C:\cmakeinstall\googletest` or `~/cmakeinstall/googletest`).
  - Set `CMAKE_BUILD_TYPE` to `Debug`.
  - Make sure `gtest_force_shared_crt` is NOT checked.

Example on Windows:
```
$ mkdir C:\cmakebuild\googletest
$ cd C:\cmakebuild\googletest
$ cmake -G "NMake Makefiles" C:\github\googletest
$ cmake-gui .
```

Example on Mac or Linux:
```
$ mkdir -p ~/cmakebuild/googletest
$ cd ~/cmakebuild/googletest
$ cmake ~/github/googletest
$ cmake-gui .
```

After setting `CMAKE_INSTALL_PREFIX` and `CMAKE_BUILD_TYPE` in the CMake graphical UI, click the **Configure** button and then the **Generate** button. Then you can build and install the library from the command line. And the slightly tricky part: you also need to rename the output library files so that CMake will find these versions when running a debug build.

On Windows:
```
$ nmake install
$ cd C:\cmakeinstall\googletest\lib
$ rename gtest.lib gtestd.lib
$ rename gtest_main.lib gtest_maind.lib
$ cd C:\cmakebuild\googletest
```

On Mac or Linux:
```
$ make install
$ cd ~/cmakeinstall/googletest/lib
$ mv libgtest.a libgtestd.a
$ mv libgtest_main.a libgtest_maind.a
$ cd ~/cmakebuild/googletest
```

Now repeat this process one more time on Mac/Linux and three more times on Windows so you end up with all of the following installed libraries:

CMAKE_BUILD_TYPE | gtest_force_shared_crt | Mac/Linux                         | Windows
-----------------|------------------------|-----------------------------------|--------------------------------------
Debug            | unchecked              | libgtestd.a <br> libgtest_maind.a | gtestd.lib <br> gtest_maind.lib
Debug            | checked                | (n/a)                             | gtest-mdd.lib <br> gtest_main-mdd.lib
Release          | checked                | (n/a)                             | gtest-md.lib <br> gtest_main-md.lib
Release          | unchecked              | libgtest.a <br> libgtest_main.a   | gtest.lib <br> gtest_main.lib


## Building oomuse-core

Once googletest is installed, you're ready to build oomuse-core. You can follow the same basic procedure:

- Clone this git repro somewhere (_e.g._ `C:\github\oomuse-core` or `~/github/oomuse-core`).
- Create a separate directory to use for building (_e.g._ `C:\cmakebuild\oomuse-core` or `~/cmakebuild/oomuse-core`).
- Set up a CMake build environment and run `cmake-gui` to customize the output location:
  - Change `CMAKE_INSTALL_PREFIX` to a separate install dir (_e.g._ `C:\cmakeinstall\oomuse-core` or `~/cmakeinstall/oomuse-core`).
  - Set `CMAKE_BUILD_TYPE` to `Debug` or `Release`. (You can repeat these steps if you want to install both variants).
  - If you're using the Microsoft Visual C++ compiler on Windows, set `OOMUSE_CRT_LINKAGE` to `dynamic` (corresponds to DLL linkage of the C runtime library, `/MDd` and `/MD` compiler flags) or `static` (corresponds to static linkage of the C runtime library, `/MTd` and `/MT` flags). This needs to match the compiler settings for whatever project you're planning to use oomuse-core from. (If later on you get a spew of linker error output complaining about missing symbols from the C standard library, try the other one).
  - Add an entry for `GTEST_ROOT` of type FILEPATH with value set to whatever you set CMAKE_INSTALL_PREFIX to for googletest above (_e.g._ `C:\cmakeinstall\googletest` or `/Users/<your_username_here>/cmakeinstall/oomuse`). Note that this path can't use `~`, so you'll need to expand the absolute path of your home directory if you installed within there as in the example paths here.

On Windows (note that the cmake command will emit an error about not being able to find GTest&mdash;that's okay; it will be resolved when you set `GTEST_ROOT` in the CMake GUI):
```
$ mkdir C:\cmakebuild\oomuse-core
$ cd C:\cmakebuild\oomuse-core
$ cmake -G "NMake Makefiles" C:\github\oomuse-core
$ cmake-gui .
```

And on Mac or Linux:
```
$ mkdir -p ~/cmakebuild/oomuse-core
$ cd ~/cmakebuild/oomuse-core
$ cmake ~/github/oomuse-core
$ cmake-gui .
```

Once you've set `CMAKE_INSTALL_PREFIX`, `GTEST_ROOT`, `CMAKE_BUILD_TYPE`, and (for Windows) `OOMUSE_CRT_LINKAGE`, again click **Configure** and then **Generate**. Go back to the command line to build and install the library.

Windows:
```
$ nmake install
```

Mac and Linux:
```
$ make install
```


## Running oomuse-core Tests

If you're just planning to use oomuse-core, this is optional, but it doesn't hurt to run tests as a sanity check that the library will work as expected on your system.

On Windows (either of these commands):
```
$ nmake test
$ oomuse-core_test
```

On Mac or Linux (either command):
```
$ make test
$ ./oomuse-core_test
```

The first command will run the tests and output success or failiure status. To see the individual tests or more details about a failure, use the second command to run the test executable directly.


## Using oomuse-core from Your Own Project

Copy [`cmake/Modules/FindOOMuseCore.cmake`](https://github.com/Lindurion/oomuse-core/blob/master/cmake/Modules/FindOOMuseCore.cmake) into your CMake project at `<your-project>/cmake/Modules/FindOOMuseCore.cmake`.

Set input CMake cache variables using `cmake-gui`:
  - `OOMUSE_ROOT`: Set this file path to the dir below the oomuse-core install dir, e.g. `C:/cmakeinstall` or `/Users/<your-username>/cmakeinstall`
  - `OOMUSE_CRT_LINKAGE`: For Windows with the Microsoft Visual C++ compiler, set to `dynamic` or `static` to control C runtime library linkage.

In your root CMakeLists.txt, add this line before using oomuse-core:
```CMake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
```

Then find the OOMuseCore package:
```CMake
find_package(OOMuseCore REQUIRED)
```

Finally, add the include dirs and link against the library for your target:
```CMake
set_property(TARGET <your-target>
    APPEND PROPERTY INCLUDE_DIRECTORIES ${OOMUSECORE_INCLUDE_DIRS})
target_link_libraries(<your-target> ${OOMUSECORE_LIBRARIES})
```

## License

This open source library is free to use in your own projects, released under the [Apache License Version 2.0](https://github.com/Lindurion/oomuse-core/blob/master/LICENSE).
