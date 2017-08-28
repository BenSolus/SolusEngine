
# SolusEngine

---

## Summary

The SolusEngine is a 3D graphics engine with
[Vulkan](https://www.khronos.org/vulkan/) support.

---

## Dependencies

*   [assimp](http://assimp.sourceforge.net/) 3.3 or newer
*   [CMake](https://cmake.org/) 3.2.2 or newer
*   [GLM](http://glm.g-truc.net/0.9.8/index.html) 0.9.5.1-1 or newer
*   [VulkanSDK](https://vulkan.lunarg.com/sdk/home) 1.0.51.0 or newer
*   [GLFW](http://www.glfw.org/) 3.2.1 or newer

Older versions might also work, but untested.

---

## Building

### Linux

[![Travis.ci Shield](https://img.shields.io/travis/BenSolus/SolusEngine/master.svg?style=plastic&label=Linux)](https://travis-ci.org/BenSolus/SolusEngine)

Following bash script should retrieve the source and compile it in debug mode:

```shell
#!/usr/bin/bash

git clone https://github.com/BenSolus/master-thesis.git

cd master-thesis || exit 1

PROJECT_DIR=${PWD}

mkdir -p "${PROJECT_DIR}/build"

cd "${PROJECT_DIR}/build" || exit 1

cmake -DCMAKE_BUILD_TYPE=Debug ../..

make -j2
```

---

## Roadmap

-   [x] Vulkan interface
-   [ ] Vulkan interface improvments
-   [ ] Vulkan interface testing
-   [ ] Integrate a Vulkan memory allocator
-   [ ] Input handling
-   [ ] Input handling testing
-   [ ] 3D Models
-   [ ] Shader
-   [ ] Integrate a physic engine

Currently, this is only a very rough, non-exhaustive overview of things needed
to be implemented. Further ideas, feedback and help are appreciated.

---

## Attribution

This project, "SolusEngine",
*   is heavily inspired by the
    [code](https://github.com/Overv/VulkanTutorial/tree/master/code) of this
    [Vulkan Tutorial](https://vulkan-tutorial.com/) by
    [Alexander Overvoorde](https://github.com/Overv), used under
    [CC0 1.0 Universal](https://creativecommons.org/publicdomain/zero/1.0/),
*   uses [stb](https://github.com/nothings/stb)(_image.h) for image processing
    by [Sean Barrett](https://github.com/nothings), used under
    [CC0 1.0 Universal](https://creativecommons.org/publicdomain/zero/1.0/).

We don't imply any endorsement by the authors or any affirmer.
