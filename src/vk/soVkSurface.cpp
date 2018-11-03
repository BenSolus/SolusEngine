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

#include "cxx/soMemory.hpp"

#include <regex>

class
so::vk::Surface::Impl
{
    using SurfaceHandle    = void*;
    using SurfaceProviders =
            std::vector<std::pair<Module, SurfaceHandle>>;

  public:
    Impl() : mProviders(0)
    {
      Path const configPath
                   { getBinaryDir() / "data" / "backends" / "surface" };

      std::regex const match{ "(.*).json$" };

      for(auto const& file : DirectoryIterator{ configPath })
      {
        so::Path const config{ file.path() };

        if(std::regex_match(config.string(), match))
        {
          mProviders.push_back({ Module(config, EngineBackend::Vulkan),
                                 nullptr });
        }
      }
    }

    Impl(Impl const& other) = delete;

    Impl(Impl&& other) noexcept = delete;

    ~Impl() noexcept
    {
      for(auto& provider: mProviders)
      {
        if(provider.first.isAvailable())
        {
          provider.first(1, VoidReturn::getVoidReturn(), provider.second);
        }
      }
    }

    Impl&
    operator=(Impl const& other) = delete;

    Impl&
    operator=(Impl&& other) noexcept = delete;

    so::return_t
    initialize()
    {
      bool   atLeastOneValidBackend(false);
  
      index_t idx(0);
  
      for(auto& provider : mProviders)
      {
        if(provider.first.isAvailable())
        {
          std::string verbose("<VERBOSE> Starting initialization of surface "
                              "provider '");

          verbose += provider.first.getName();
          verbose += "' for engine backend 'Vulkan'.\n";

          std::cout << verbose;

          so::return_t result;
 
          provider.first(0, result, &provider.second);

          if(result is_eq failure)
          {
            verbose += "<ERROR>   Failed to initialize surface provider '";
            verbose += provider.first.getName();
            verbose += "' for engine backend 'Vulkan'.";

            continue;
          }
          
          verbose  = "<VERBOSE> Initialized surface provider '";

          verbose += provider.first.getName();
          verbose += "' for engine backend 'Vulkan'.\n";

          std::cout << verbose;

          if(not atLeastOneValidBackend)
          {
            mCurrentProvider       = idx;
          
            atLeastOneValidBackend = true;

            verbose  = "<VERBOSE> Using surface provider '";
          
            verbose += provider.first.getName();
            verbose += "' in engine backend 'Vulkan'.\n";

            std::cout << verbose;
          }
        }

        ++idx;
      }

      if(not atLeastOneValidBackend)
      {
        return failure;
      }

      return success;
    }

    return_t
    getInstanceExtensions(std::vector<char const*>& instanceExtensions) const
    {
      instanceExtensions = std::vector<char const*>();

      for(auto const& provider : mProviders)
      {
        return_t     result;
        char const** providerExtensions{ nullptr };
        size_type    count;

        provider.first(2,
                       result,
                       provider.second,
                       &providerExtensions,
                       &count);

        if(result is_eq failure)
        {
          std::string error{ "<ERROR>   Failed to get instance extensions " };

          error += "for surface provider '";
          error += provider.first.getName();
          error += "'.";

          std::cout << error << '\n';

          continue;
        }

        std::vector<char const*> providerInstanceExtensions
                                   { providerExtensions,
                                     providerExtensions + count };
                
        instanceExtensions.insert(instanceExtensions.end(),
                                  providerInstanceExtensions.begin(),
                                  providerInstanceExtensions.end());
      }

      return success;
    }

    return_t
    createWindow(std::string const& title,
                 size_type   const  width,
                 size_type   const  height)
    {
      auto const idx{ static_cast<SurfaceProviders::size_type>
                        (mCurrentProvider) };

      auto& module{ mProviders[idx].first };

      auto  handle{ mProviders[idx].second };
      
      return_t result{ failure };

      module(3, result, handle, title, width, height);

      return result;
    }

    return_t
    createSurface(VkInstance instance)
    {
      auto const idx{ static_cast<SurfaceProviders::size_type>
                        (mCurrentProvider) };

      auto& module{ mProviders[idx].first };

      auto  handle{ mProviders[idx].second };
 
      return_t result{ failure };

      module(4, result, handle, instance);

      return result;
    }

    VkSurfaceKHR
    getVkSurfaceKHR()
    {
      auto& module{ getModule() };
      
      auto  handle{ getHandle() };

      VkSurfaceKHR surface;

      module(5, surface, handle);

      return surface;
    }

    bool
    windowIsClosed()
    {
      auto& module{ getModule() };
      
      auto  handle{ getHandle() };

      bool isClosed(true);

      module(6, isClosed, handle);

      return isClosed;
    }

    void
    pollEvents()
    {
      getModule()(7, VoidReturn::getVoidReturn());
    }

    void
    getWindowSize(size_type& width, size_type& height)
    {
      auto& module{ getModule() };

      auto  handle{ getHandle() };

      module(8, VoidReturn::getVoidReturn(), handle, &width, &height);
    }

    bool
    framebuffersAreResized()
    {
      bool areResized;

      getModule()(9, areResized, getHandle());
    
      return areResized;
    }

    std::string const
    getName()
    {
      return getModule().getName();
    }

    bool
    isAvailable()
    {
      return getModule().isAvailable();
    }

  private:
    SurfaceProviders mProviders{ 0 };

    index_t          mCurrentProvider{ -1 };

    inline Module&
    getModule()
    {
      auto idx{ static_cast<SurfaceProviders::size_type>(mCurrentProvider) };

      return mProviders[idx].first;
    }

    inline SurfaceHandle
    getHandle()
    {
      auto idx{ static_cast<SurfaceProviders::size_type>(mCurrentProvider) };

      return mProviders[idx].second;
    }
};

so::vk::Surface::Surface()
  : mPImpl(make_unique<Impl>()),
    mInstance(Instance::getSharedPtrNullInstance())
{}

so::vk::Surface::~Surface() noexcept = default;

so::return_t
so::vk::Surface::initialize()
{
  return mPImpl->initialize();
}

so::return_t
so::vk::Surface::getInstanceExtensions
  (std::vector<char const*>& instanceExtensions)
{
  return mPImpl->getInstanceExtensions(instanceExtensions);
}

so::return_t
so::vk::Surface::createWindow(std::string const& title,
                              size_type   const  width,
                              size_type   const  height)
{
  return mPImpl->createWindow(title, width, height);
}

so::return_t
so::vk::Surface::createSurface()
{
  return mPImpl->createSurface(mInstance->getVkInstance());
}

bool
so::vk::Surface::windowIsClosed()
{ 
  return mPImpl->windowIsClosed();
}

void
so::vk::Surface::pollEvents()
{
  mPImpl->pollEvents();
}

VkSurfaceKHR
so::vk::Surface::getVkSurfaceKHR() const
{
  return mPImpl->getVkSurfaceKHR();
}

void
so::vk::Surface::getWindowSize(size_type& width, size_type& height) const
{
  mPImpl->getWindowSize(width, height);
}

bool
so::vk::Surface::framebuffersAreResized()
{
  return mPImpl->framebuffersAreResized();
}

void
so::vk::Surface::setSharedPtrInstance(SharedPtrInstance const& instance)
{
  mInstance = instance;
}

so::vk::SharedPtrInstance
so::vk::Surface::getSharedPtrInstance()
{
  return mInstance;
}

