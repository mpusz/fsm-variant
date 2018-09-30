# Installation and Reuse

There are a few different ways of installing/reusing `mp::fsm` in your project 

## Copy

`mp::fsm` is a standalone header-only library without any additional dependencies.
It can be simply copied to your source tree and used as a regular header file.

## cmake

To use `mp::fsm` as a `cmake` imported library exported as `cmake` target via `cmake`
configuration files following steps may be done.

### cmake install

```bash
mkdir build && cd build
cmake ../src -DCMAKE_INSTALL_PREFIX=<your_cmake_installation_dir> <your_cmake_configuration>
cmake --build . --target install <your_cmake_configuration>
```

To use such `cmake` target in your project it is enough to add following line to your `CMakeList.txt` file

```cmake
find_package(fsm CONFIG REQUIRED)
```

and configure it with

```bash
cmake .. -DCMAKE_INSTALL_PREFIX=<your_cmake_installation_dir> <your_cmake_configuration>
```

### cmake + conan

To use `mp::fsm` with `cmake` via `conan` it is enough to:
- add following remote to your local `conan` instance

```bash
conan remote add conan-mpusz https://api.bintray.com/conan/mpusz/conan-mpusz
```

- add following dependency to your `conanfile.txt` or `conanfile.py` files:

```python
requires = "fsm-variant/1.0.0@mpusz/stable"
```
