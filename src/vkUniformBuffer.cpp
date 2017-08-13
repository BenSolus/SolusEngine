/*
 * Copyright (c) 2017 Bennet Carstensen
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vkUniformBuffer.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

vk::eng::UniformBuffer::UniformBuffer()
  : Buffer()
{}

vk::eng::UniformBuffer::UniformBuffer(SharedPtrLogicalDevice const& device)
  : Buffer(device,
           sizeof(UniformBufferObject),
           VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
           VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{}

void
vk::eng::UniformBuffer::update(VkExtent2D swapChainExtent)
{
  static auto startTime(std::chrono::high_resolution_clock::now());

  auto currentTime(std::chrono::high_resolution_clock::now());

  float time =
    static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>
                         (currentTime - startTime).count()) / 1000.0f;

  UniformBufferObject ubo = {};

  ubo.model = glm::rotate(glm::mat4(),
                          time * glm::radians(90.0f),
                          glm::vec3(0.0f, 1.0f, 0.0f));

  ubo.view  = glm::lookAt(glm::vec3(-2.0f, -2.0f, -2.0f),
                          glm::vec3(0.0f, 0.0f, 0.0f),
                          glm::vec3(0.0f, -1.0f, 0.0f));

  ubo.proj  = glm::perspective
                (glm::radians(45.0f),
                 static_cast<float>(swapChainExtent.width) /
                 static_cast<float>(swapChainExtent.height),
                 0.1f,
                 10.0f);

  ubo.proj[1][1] *= -1;

  VkDevice device(mDevice->getVkDevice());

  void* data;

  vkMapMemory(device, mDeviceMemory, 0, sizeof(ubo), 0, &data);

  memcpy(data, &ubo, sizeof(ubo));

  vkUnmapMemory(device, mDeviceMemory);
}
