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

/**
 *  @file      vk/soVkInstance.hpp
 *  @author    Bennet Carstensen
 *  @date      2017
 *  @copyright Copyright (c) 2017-2018 Bennet Carstensen
 *
 *             Permission is hereby granted, free of charge, to any person
 *             obtaining a copy of this software and associated documentation
 *             files (the "Software"), to deal in the Software without
 *             restriction, including without limitation the rights to use,
 *             copy, modify, merge, publish, distribute, sublicense, and/or
 *             sell copies of the Software, and to permit persons to whom the
 *             Software is furnished to do so, subject to the following
 *             conditions:
 *
 *             The above copyright notice and this permission notice shall be
 *             included in all copies or substantial portions of the Software.
 *
 *             THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *             EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *             OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *             NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *             HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *             WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *             FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *             OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include "soReturnT.hpp"

#include <vulkan/vulkan.hpp>

#include <memory>
#include <vector>

#ifdef NDEBUG
constexpr bool ENABLE_VALIDATION_LAYERS{ false };
#else
constexpr bool ENABLE_VALIDATION_LAYERS{ true };
#endif

extern std::array<char const*, 1> const VALIDATION_LAYERS;

namespace so {
namespace vk {
    
class
Instance : public std::enable_shared_from_this<Instance>
{
  using Extensions = std::vector<char const*>;

  public:
    static std::shared_ptr<Instance> const
    getSharedPtrNullInstance();

    Instance();
 
    Instance(Instance const& other) = delete;

    Instance(Instance&& other) noexcept = delete;

    ~Instance() noexcept;

    Instance& operator=(Instance const& other) = delete;

    Instance& operator=(Instance&& other) noexcept = delete;

    return_t
    initialize(std::string const& applicationName,
               uint32_t    const  applicationVersion,
               Extensions  const& additionalExtensions);

    inline VkInstance getVkInstance() { return mInstance; }

    inline void setVkInstance(VkInstance instance) { mInstance = instance; }

  private:
    VkInstance mInstance{ VK_NULL_HANDLE };

    std::vector<const char*>
    getRequiredExtensions();

    bool
    checkValidationLayerSupport();

}; // class Instance

using SharedPtrInstance = std::shared_ptr<Instance>;

} // namespace vk
} // namespace so

