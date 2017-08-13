/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      vkInstance.hpp
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

#include <vulkan/vulkan.hpp>

#include <memory>
#include <vector>

#ifdef NDEBUG
    const bool ENABLE_VALIDATION_LAYERS(false);
#else
    const bool ENABLE_VALIDATION_LAYERS(true);
#endif

extern const std::vector<const char*> VALIDATION_LAYERS;

namespace vk
{
  namespace eng
  {
    class
    Instance : public std::enable_shared_from_this<Instance>
    {
      public:
        Instance();

        Instance(VkInstance instance) : mInstance(instance) {}

        Instance(Instance const& other) = delete;

        Instance(Instance&& other) = delete;

        ~Instance() noexcept;

        Instance& operator=(Instance const& other) = delete;

        Instance& operator=(Instance&& other) = delete;

        inline VkInstance getVkInstance() { return mInstance; }

        inline void setVkInstance(VkInstance instance)
        { mInstance = instance; }

      private:
        VkInstance mInstance;

        std::vector<const char*>
        getRequiredExtensions();

        bool
        checkValidationLayerSupport();
    };

    using SharedPtrInstance = std::shared_ptr<Instance>;
  } // namespace eng
} //namespace vk

extern vk::eng::SharedPtrInstance const SHARED_PTR_NULL_INSTANCE;
