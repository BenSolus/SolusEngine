/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      VkShaderModule.hpp
 *  @author    Bennet Carstensen
 *  @date      2017
 *  @copyright Copyright (c) 2017 Bennet Carstensen
 *
 *             This Source Code Form is subject to the terms of the
 *             Mozilla Public License, v. 2.0. If a copy of the MPL was
 *             not distributed with this file, You can obtain one at
 *             http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <vk/soVkLogicalDevice.hpp>
#include <vk/soVkSwapChain.hpp>

#include <string>
#include <vector>

namespace so
{
  namespace vk
  {
    class
    ShaderModule
    {
      public:
        ShaderModule();

        ShaderModule(SharedPtrLogicalDevice const& device,
                     const std::string&            file);

        ShaderModule(ShaderModule const& other) = delete;

        ShaderModule(ShaderModule&& other) = delete;

        ~ShaderModule() noexcept;

        ShaderModule& operator=(ShaderModule const& other) = delete;

        ShaderModule&
        operator=(ShaderModule&& other) noexcept
        {
          destroy_members();

          mShaderModule = other.mShaderModule;
          mDevice       = other.mDevice;

          other.mShaderModule = VK_NULL_HANDLE;
          other.mDevice       = SHARED_PTR_NULL_LOGICAL_DEVICE;

          return *this;
        }

        inline VkShaderModule getVkShaderModule() { return mShaderModule; }

      private:
        VkShaderModule         mShaderModule;

        SharedPtrLogicalDevice mDevice;

        void
        destroy_members();
    };
  } // namespace vk
} // namespace so
