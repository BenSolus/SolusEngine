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
        if(provider.first.isAvailable())
          provider.first(1, VoidReturn::getVoidReturn(), provider.second);
    }

    Impl&
    operator=(Impl const& other) = delete;

    Impl&
    operator=(Impl&& other) noexcept = delete;

    void
    initialize()
    {
      bool   atLeastOneValidBackend(false);
  
      index idx(0);
  
      for(auto& provider : mProviders)
      {
        if(provider.first.isAvailable())
        {
          try
          {
          std::string verbose("<VERBOSE> Starting initialization of surface "
                              "provider '");

          verbose += provider.first.getName();
          verbose += "' for engine backend 'Vulkan'.\n";

          std::cout << verbose;

          char const* error;
 
          provider.first(0, error, &provider.second);

          if(error not_eq nullptr)
            THROW_EXCEPTION(error);

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
          catch(...)
          {
          std::string warning("<WARNING> Failed initialization of surface "
                              " backend '");

          warning += provider.first.getName();
          warning += "' for engine backend 'Vulkan'.\n";

          std::cerr << warning;
          }
        }

        ++idx;
      }

      if(not atLeastOneValidBackend)
      {
        THROW_EXCEPTION("Couldn't initialize any surface provider.");
      }

    }

    std::vector<char const*>
    getInstanceExtensions() const
    {
      std::vector<char const*> allProviderInstanceExtensions;

      for(auto const& provider : mProviders)
      {
        char const*  error{ nullptr };
        char const** extensions{ nullptr };
        size_type    count;

        provider.first(2, error, provider.second, &extensions, &count);

        if(error not_eq nullptr)
        {
          THROW_EXCEPTION(error);
        }

        std::vector<char const*> providerInstanceExtensions
                                   { extensions, extensions + count };
                
        allProviderInstanceExtensions.insert
                                        (allProviderInstanceExtensions.end(),
                                         providerInstanceExtensions.begin(),
                                         providerInstanceExtensions.end());
      }

      return allProviderInstanceExtensions;
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

    std::string const&
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

    index            mCurrentProvider{ -1 };

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
  : mPImpl(std::make_unique<Impl>()),
    mInstance(Instance::SHARED_PTR_NULL_INSTANCE)
{}

so::vk::Surface::~Surface() noexcept = default;

void
so::vk::Surface::initialize()
{
  mPImpl->initialize();
}

std::vector<char const*>
so::vk::Surface::getInstanceExtensions()
{
  return mPImpl->getInstanceExtensions();
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

