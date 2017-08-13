/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file
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

#include <vkInstance.hpp>

#include <memory>
#include <vector>

namespace vk
{
  namespace eng
  {
    class
    DebugReportCallbackEXT
    {
      public:
        DebugReportCallbackEXT();

        DebugReportCallbackEXT(SharedPtrInstance const& instance);

        DebugReportCallbackEXT(DebugReportCallbackEXT const& other) = delete;

        DebugReportCallbackEXT(DebugReportCallbackEXT&& other) = delete;

        ~DebugReportCallbackEXT() noexcept;

        DebugReportCallbackEXT&
        operator=(DebugReportCallbackEXT const& other) = delete;

        DebugReportCallbackEXT&
        operator=(DebugReportCallbackEXT&& other) noexcept
        {
          mCallback = other.mCallback;
          mInstance = other.mInstance;

          other.mCallback = nullptr;
          other.mInstance = SHARED_PTR_NULL_INSTANCE;

          return *this;
        }

        VkDebugReportCallbackEXT&
        getVkDebugReportCallbackEXT() { return mCallback; }

      private:
        VkDebugReportCallbackEXT mCallback;
        SharedPtrInstance        mInstance;
    };

    bool
    checkValidationLayerSupport();
  } // namespace eng
} // namespace vk
