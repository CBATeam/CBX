# CBX

This project utilises the [IDI CPP Template]((https://github.com/IDI-Systems/idi-cpp-template) licensed under MIT.

## Build Instructions

### Requirements

- CMake >= 3.14
- GCC / Clang / MSVC (VS2019)

Make sure you have initialized submodules:
```
$ git submodule init
$ git submodule update
```

From root directory of project checkout run the following commands:
```
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .   # or generated build system command (make, Visual Studio etc.)
```

Run tests:
```
$ ctest -C Debug
```
