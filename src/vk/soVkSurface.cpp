/*
 * Copyright (C) 2017-2018 by Bennet Carstensen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <soVkSurface.hpp>

#include <soException.hpp>

#include <regex>

so::vk::SurfaceBackend::SurfaceBackend()
  : mModule(), mHandle(nullptr)
{}

so::vk::SurfaceBackend::SurfaceBackend(Path const& config)
  : mModule(config, EngineBackend::Vulkan), mHandle(nullptr)
{}

so::vk::SurfaceBackend::SurfaceBackend(SurfaceBackend&& other) noexcept
  : mModule(std::move(other.mModule)), mHandle(other.mHandle)
{
  other.mHandle = nullptr;
}

so::vk::SurfaceBackend::~SurfaceBackend() noexcept
{
  if(mModule.isAvailable())
    mModule(1, VoidReturn::getVoidReturn(), mHandle);
}

void
so::vk::SurfaceBackend::initialize()
{
  char const* error;
 
  mModule(0, error, &mHandle);

  if(error not_eq nullptr)
    THROW_EXCEPTION(error); 
}

std::vector<char const*>
so::vk::SurfaceBackend::getInstanceExtensions() const
{
  char const*  error{ nullptr };
  char const** extensions{ nullptr };
  size_type    count;

  mModule(2, error, mHandle, &extensions, &count);

  if(error not_eq nullptr)
  {
    THROW_EXCEPTION(error);
  }

  std::vector<char const*> extensionVector(extensions, extensions + count);

  return extensionVector;
}

so::return_t
so::vk::SurfaceBackend::createWindow(std::string const& title,
                                     size_type   const  width,
                                     size_type   const  height)
{
  return_t result{ failure };

  mModule(3, result, mHandle, title, width, height);

  return result;
}

so::return_t
so::vk::SurfaceBackend::createSurface(VkInstance instance)
{
  return_t result{ failure };

  mModule(4, result, mHandle, instance);

  return result;
}

VkSurfaceKHR
so::vk::SurfaceBackend::getVkSurfaceKHR()
{
  VkSurfaceKHR surface;

  mModule(5, surface, mHandle);

  return surface;
}

bool
so::vk::SurfaceBackend::windowIsClosed()
{
  bool isClosed(true);

  mModule(6, isClosed, mHandle);

  return isClosed;
}

void
so::vk::SurfaceBackend::pollEvents()
{
  mModule(7, VoidReturn::getVoidReturn());
}

std::string const&
so::vk::SurfaceBackend::getName()
{
  return mModule.getName();
}

bool
so::vk::SurfaceBackend::isAvailable()
{
  return mModule.isAvailable();
}

so::vk::Surface::Surface()
  : mBackends(0),
    mActiveBackend(0),
    mInstance(Instance::SHARED_PTR_NULL_INSTANCE)
{
  Path const configPath(getBinaryDir() / "data" / "backends" / "surface");

  for(auto const& file : DirectoryIterator(configPath))
  {
    std::regex const match("(.*).json$");

    so::Path const config(file.path());

    if(std::regex_match(config.string(), match))
    {
      mBackends.push_back(SurfaceBackend(config));    	
    }
  }
}

void
so::vk::Surface::initialize()
{
  bool   atLeastOneValidBackend(false);
  
  index idx(0);
  
  for(auto& backend : mBackends)
  {
    if(backend.isAvailable())
    {
      try
      {
        std::string verbose("<VERBOSE> Starting initialization of surface "
                            "backend '");

        verbose += backend.getName();
        verbose += "' for engine backend 'Vulkan'.\n";

        std::cout << verbose;

        backend.initialize();

        verbose  = "<VERBOSE> Initialized surface backend '";

        verbose += backend.getName();
        verbose += "' for engine backend 'Vulkan'.\n";

        std::cout << verbose;

        if(not atLeastOneValidBackend)
        {
          mActiveBackend         = idx;
          
          atLeastOneValidBackend = true;

          verbose  = "<VERBOSE> Using surface backend '";
          
          verbose += backend.getName();
          verbose += "' in engine backend 'Vulkan'.\n";

          std::cout << verbose;
        }
      }
      catch(...)
      {
        std::string warning("<WARNING> Failed initialization of surface "
                            " backend '");

        warning += backend.getName();
        warning += "' for engine backend 'Vulkan'.\n";

        std::cerr << warning;
      }
    }

    ++idx;
  }

  if(not atLeastOneValidBackend)
  {
    THROW_EXCEPTION("Couldn't initialize any surface backend.");
  }
}

std::vector<char const*>
so::vk::Surface::getInstanceExtensions()
{
  std::vector<char const*> allInstanceExtensions(0);

  for(auto const& backend : mBackends)
  {
    auto const backendInstanceExtensions(backend.getInstanceExtensions());

    allInstanceExtensions.insert(allInstanceExtensions.end(),
                                 backendInstanceExtensions.begin(),
                                 backendInstanceExtensions.end());
  }

  return allInstanceExtensions;
}

so::return_t
so::vk::Surface::createWindow(std::string const& title,
                              size_type   const  width,
                              size_type   const  height)
{
  auto const idx{ static_cast<Backends::size_type>(mActiveBackend) };

  return mBackends[idx].createWindow(title, width, height);
}

so::return_t
so::vk::Surface::createSurface()
{
  auto const idx{ static_cast<Backends::size_type>(mActiveBackend) };

  return mBackends[idx].createSurface(mInstance->getVkInstance());
}

bool
so::vk::Surface::windowIsClosed()
{
  auto const idx{ static_cast<Backends::size_type>(mActiveBackend) };
  
  return mBackends[idx].windowIsClosed();
}

void
so::vk::Surface::pollEvents()
{
  auto const idx{ static_cast<Backends::size_type>(mActiveBackend) };
  
  mBackends[idx].pollEvents();
}

VkSurfaceKHR
so::vk::Surface::getVkSurfaceKHR()
{
  auto const idx{ static_cast<Backends::size_type>(mActiveBackend) };

  return mBackends[idx].getVkSurfaceKHR();
}

GLFWwindow*
so::vk::Surface::getGLFWwindow()
{
  return nullptr;
}

void
so::vk::Surface::setSharedPtrInstance(SharedPtrInstance instance)
{
  mInstance = instance;
}

so::vk::SharedPtrInstance
so::vk::Surface::getSharedPtrInstance()
{
  return mInstance;
}

