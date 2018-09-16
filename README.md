
# SolusEngine

---

## Summary

The SolusEngine is a hobby project, trying to implement a 3D graphics engine.
Currently focusing on an Implementation of a
[Vulkan](https://www.khronos.org/vulkan/)-Backend for the engine.

---

## Dependencies

*   [assimp](http://assimp.sourceforge.net/) 3.3 or newer
*   [CMake](https://cmake.org/) 3.2.2 or newer
*   [GLM](http://glm.g-truc.net/0.9.8/index.html) 0.9.5.1-1 or newer
*   [VulkanSDK](https://vulkan.lunarg.com/sdk/home) 1.0.51.0 or newer
*   [GLFW](http://www.glfw.org/) 3.2.1 or newer
*   [Google Test](https://github.com/google/googletest) 1.8 or newer

Older versions might also work, but untested.

---

## Building

### Linux

[![Travis.ci Shield](https://img.shields.io/travis/BenSolus/SolusEngine/master.svg?style=plastic&label=Ubuntu-14.04)](https://travis-ci.org/BenSolus/SolusEngine)

Following bash script retrieves the source and compiles it, including the
examples, replacing <Build-Mode> with the desired mode to build, e.g. Debug or
Release:

```shell
#!/usr/bin/bash

git clone https://github.com/BenSolus/SolusEngine.git

cd SolusEngine || exit 1

PROJECT_DIR=${PWD}

mkdir -p "${PROJECT_DIR}/build"

cd "${PROJECT_DIR}/build" || exit 1

cmake -DCMAKE_BUILD_TYPE=Debug -DEXAMPLES:BOOL=ON ../..

make -j2
```

---

## Attribution

This project, "SolusEngine",
*   is heavily inspired by the
    [code](https://github.com/Overv/VulkanTutorial/tree/master/code) of this
    [Vulkan Tutorial](https://vulkan-tutorial.com/) by
    [Alexander Overvoorde](https://github.com/Overv), used under
    [CC0 1.0 Universal](https://creativecommons.org/publicdomain/zero/1.0/),
*   uses [JSON for Modern C++](https://github.com/nlohmann/json) by
    [Niels Lohmann](http:/nlohmann.me), used under the
    [MIT License](https://opensource.org/licenses/MIT).
*   uses [stb](https://github.com/nothings/stb)(_image.h) for image processing
    by [Sean Barrett](https://github.com/nothings), used under
    [CC0 1.0 Universal](https://creativecommons.org/publicdomain/zero/1.0/).

I don't imply any endorsement by the authors or any affirmer.

