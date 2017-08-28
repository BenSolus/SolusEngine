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

#include <vk/soVkTextureSampler.hpp>

so::vk::TextureSampler::TextureSampler()
  : mSampler(VK_NULL_HANDLE),
    mTextureImageViews(),
    mTextureImages(),
    mDevice(SHARED_PTR_NULL_LOGICAL_DEVICE),
    mCommandPool(SHARED_PTR_NULL_COMMAND_POOL)
{}

so::vk::TextureSampler::TextureSampler
  (SharedPtrLogicalDevice const& device,
   SharedPtrCommandPool const&   commandPool)
  : mSampler(VK_NULL_HANDLE),
    mTextureImageViews(device),
    mTextureImages(),
    mDevice(device),
    mCommandPool(commandPool)
{
  VkSamplerCreateInfo samplerInfo = {};

  samplerInfo.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  samplerInfo.magFilter               = VK_FILTER_LINEAR;
  samplerInfo.minFilter               = VK_FILTER_LINEAR;
  samplerInfo.addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.anisotropyEnable        = VK_TRUE;
  samplerInfo.maxAnisotropy           = 16;
  samplerInfo.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
  samplerInfo.unnormalizedCoordinates = VK_FALSE;
  samplerInfo.compareEnable           = VK_FALSE;
  samplerInfo.compareOp               = VK_COMPARE_OP_ALWAYS;
  samplerInfo.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;

  VkResult const result(vkCreateSampler(mDevice->getVkDevice(),
                                        &samplerInfo,
                                        nullptr,
                                        &mSampler));

  if(result not_eq VK_SUCCESS)
    throw utils::err::Exception<std::runtime_error>("failed to create texture "
                                                    "sampler!",
                                                    PRETTY_FUNCTION_SIG);
}

so::vk::TextureSampler::~TextureSampler() noexcept { destroyMembers(); }

bool
so::vk::TextureSampler::addTexture(std::string const path,
                                    std::string const key)
{
  std::string realKey(key.empty() ? std::to_string(mTextureImages.size())
                                  : key);

  try
  {
    auto const result(mTextureImages.insert
      (std::pair<std::string, TextureImage>
         (realKey, TextureImage(mDevice,
                                mCommandPool,
                                path,
                                mTextureImageViews.getVkImageViews().size()))));

    /* 'result' is a pair of an iterator pointing to either the newly inserted
     * element or to the element with an equivalent key and a bool set to true
     * if a new element was inserted or false if an equivalent key already
     * existed. */
    if(not result.second)
      throw utils::err::Exception<std::runtime_error>("key already taken by a "
                                                      "texture image!",
                                                      PRETTY_FUNCTION_SIG);

    mTextureImageViews.addImageViews
      ({ result.first->second.getResource() },
       VK_FORMAT_R8G8B8A8_UNORM,
       VK_IMAGE_ASPECT_COLOR_BIT);

    return result.second;
  }
  catch(...)
  {
    std::throw_with_nested(utils::err::Exception<std::runtime_error>
      ("failed to load Texture \"" +
       realKey +
       "\" (" +
       path +
       ")",
       PRETTY_FUNCTION_SIG));
  }

  /* Shouldn't reach this statement, but it will silence some
   * compilers/linters. */
  return false;
}

void
so::vk::TextureSampler::destroyMembers()
{
  VkDevice device(mDevice->getVkDevice());

  if((device not_eq VK_NULL_HANDLE) and (mSampler not_eq VK_NULL_HANDLE))
    vkDestroySampler(device, mSampler, nullptr);
}