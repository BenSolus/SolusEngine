/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 *  @file      soVkQueueFamilyIndices.hpp
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

#include <vulkan/vulkan.h>

namespace so
{
  namespace vk
  {
    class
    QueueFamilyIndices
    {
      public:
        QueueFamilyIndices() : mGraphicsFamily(-1), mPresentFamily(-1) {}

        QueueFamilyIndices(VkPhysicalDevice device, VkSurfaceKHR surface);

        inline int getGraphicsFamily() { return mGraphicsFamily; }

        inline void setGraphicsFamily(int value) { mGraphicsFamily = value; }

        inline int getPresentFamily() { return mPresentFamily; }

        inline void setPresentFamily(int value) { mPresentFamily = value; }

        inline bool isComplete()
        { return (mGraphicsFamily >= 0) && (mPresentFamily >= 0); }

      private:
        int mGraphicsFamily;
        int mPresentFamily;
    };
  } // namespace vk
} //namespace so