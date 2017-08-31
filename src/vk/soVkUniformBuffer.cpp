/*
 * Copyright (C) 2017 by Bennet Carstensen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <vk/soVkUniformBuffer.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include <random>

static glm::vec3 rotations[125];
static glm::vec3 rotationSpeeds[125];

so::vk::UniformBuffer::UniformBuffer()
  : mViewProjectionUBO({}),
    mViewProjectionUBOBuffer(),
    mDynamicUBOs(),
    mDynamicBuffer(),
    mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE)
{}

so::vk::UniformBuffer::UniformBuffer
  (SharedPtrLogicalDevice const& device,
   const std::size_t             dynamicAlignment,
   const std::size_t             numDynamicUBOs)
  : mViewProjectionUBO({}),
    mViewProjectionUBOBuffer(device,
                             sizeof(ViewProjectionUBO),
                             VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                             VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
    mDynamicUBOs(dynamicAlignment, numDynamicUBOs),
    mDynamicBuffer(device,
                   numDynamicUBOs * dynamicAlignment,
                   VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT),
    mDevice(device)
{
  if(numDynamicUBOs not_eq 0)
  {
    static auto startTime(std::chrono::high_resolution_clock::now());

    auto currentTime(std::chrono::high_resolution_clock::now());

    float time =
      static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>
        (currentTime - startTime).count()) / 1000.0f;

    std::mt19937 rndGen(static_cast<uint32_t>(time));

    std::normal_distribution<float> rndDist(-1.0f, 1.0f);

    for (uint32_t i = 0; i < 125; i++)
    {
      rotations[i] = glm::vec3(rndDist(rndGen), rndDist(rndGen), rndDist(rndGen)) * 2.0f * (float)M_PI;
      rotationSpeeds[i] = glm::vec3(rndDist(rndGen), rndDist(rndGen), rndDist(rndGen));
    }
  }
}

void
so::vk::UniformBuffer::updateViewProjectionUBO(VkExtent2D swapChainExtent)
{
  mViewProjectionUBO.view  = glm::lookAt(glm::vec3(0.0f, -5.0f, -5.0f),
                                         glm::vec3(0.0f, 0.0f, 0.0f),
                                         glm::vec3(0.0f, -1.0f, 0.0f));

  mViewProjectionUBO.proj  = glm::perspective
    (glm::radians(45.0f),
     static_cast<float>(swapChainExtent.width) /
     static_cast<float>(swapChainExtent.height),
     0.1f,
     50.0f);

  mViewProjectionUBO.proj[1][1] *= -1;

  VkDevice device(mDevice->getVkDevice());

  void* data;

  vkMapMemory(device,
              mViewProjectionUBOBuffer.getVkDeviceMemory(),
              0,
              sizeof(ViewProjectionUBO),
              0,
              &data);

  memcpy(data, &mViewProjectionUBO, sizeof(ViewProjectionUBO));

  vkUnmapMemory(device, mViewProjectionUBOBuffer.getVkDeviceMemory());
}

void
so::vk::UniformBuffer::update()
{
  static auto startTime(std::chrono::high_resolution_clock::now());

  auto currentTime(std::chrono::high_resolution_clock::now());

  float time =
    static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>
                         (currentTime - startTime).count()) / 1000.0f;

  glm::vec3 offset(5.0f);

  for(std::size_t i(0); i < mDynamicUBOs.size(); ++i)
  {
    float array[] { 1.0f, 0.0f, 0.0f,     0.0f,
                    0.0f, 1.0f, 0.0f,     0.0f,
                    0.0f, 0.0f, 1.0f,     0.0f,
                    0.0f, 0.0f, 2.5f * i, 1.0f };

    glm::mat4 pos(glm::make_mat4(array));

    mDynamicUBOs[i].model = glm::rotate
      (pos,
       (i % 2 ? 1 : -1) * time * glm::radians(90.0f),
       glm::vec3(0.0f, 1.0f, 0.0f));
  }

  VkDevice device(mDevice->getVkDevice());

  void* data;

  vkMapMemory(device,
              mDynamicBuffer.getVkDeviceMemory(),
              0,
              mDynamicUBOs.alignment() * mDynamicUBOs.size(),
              0,
              &data);

  memcpy(data,
         mDynamicUBOs.data(),
         mDynamicUBOs.alignment() * mDynamicUBOs.size());

  VkMappedMemoryRange memoryRange({ });

  memoryRange.sType  = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
  memoryRange.pNext  = nullptr;
  memoryRange.memory = mDynamicBuffer.getVkDeviceMemory();
  memoryRange.offset = 0;
  memoryRange.size   = mDynamicUBOs.alignment() * mDynamicUBOs.size();

  vkFlushMappedMemoryRanges(device, 1, &memoryRange);

  vkUnmapMemory(device, mDynamicBuffer.getVkDeviceMemory());
}
