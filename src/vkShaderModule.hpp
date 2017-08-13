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

#include <vkLogicalDevice.hpp>
#include <vkSwapChain.hpp>

#include <string>
#include <vector>

namespace vk
{
  namespace eng
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
    };
  } // namespace eng
} // namespace vk
