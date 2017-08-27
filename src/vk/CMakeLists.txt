################################################################################
# Add Vulkan interface.                                                        #
################################################################################

ADD_LIBRARY(SoVk SHARED soVkBuffer.cpp
                        soVkBuffer.hpp
                        soVkCommandBuffers.cpp
                        soVkCommandBuffers.hpp
                        soVkCommandPool.cpp
                        soVkCommandPool.hpp
                        soVkDebugReportCallbackEXT.cpp
                        soVkDebugReportCallbackEXT.hpp
                        soVkDepthImage.cpp
                        soVkDepthImage.hpp
                        soVkDescriptorPool.cpp
                        soVkDescriptorPool.hpp
                        soVkDescriptorSet.cpp
                        soVkDescriptorSet.hpp
                        soVkDescriptorSetLayout.cpp
                        soVkDescriptorSetLayout.hpp
                        soVkFramebuffers.cpp
                        soVkFramebuffers.hpp
                        soVkImage.cpp
                        soVkImage.hpp
                        soVkImageViews.cpp
                        soVkImageViews.hpp
                        soVkIndexBuffer.cpp
                        soVkIndexBuffer.hpp
                        soVkInstance.cpp
                        soVkInstance.hpp
                        soVkLogicalDevice.cpp
                        soVkLogicalDevice.hpp
                        soVkMesh.cpp
                        soVkMesh.hpp
                        soVkModel.cpp
                        soVkModel.hpp
                        soVkPhysicalDevice.cpp
                        soVkPhysicalDevice.hpp
                        soVkPipeline.cpp
                        soVkPipeline.hpp
                        soVkQueueFamilyIndices.cpp
                        soVkQueueFamilyIndices.hpp
                        soVkRenderPass.cpp
                        soVkRenderPass.hpp
                        soVkResource.hpp
                        soVkSemaphore.cpp
                        soVkSemaphore.hpp
                        soVkShaderModule.cpp
                        soVkShaderModule.hpp
                        soVkStagingBuffer.hpp
                        soVkStagingBuffer.cpp
                        soVkSurface.cpp
                        soVkSurface.hpp
                        soVkSwapChain.cpp
                        soVkSwapChain.hpp
                        soVkSwapChainSupportDetails.cpp
                        soVkSwapChainSupportDetails.hpp
                        soVkTextureImage.cpp
                        soVkTextureImage.hpp
                        soVkTextureSampler.cpp
                        soVkTextureSampler.hpp
                        soVkUniformBuffer.cpp
                        soVkUniformBuffer.hpp
                        soVkVertex.cpp
                        soVkVertex.hpp
                        soVkVertexBuffer.cpp
                        soVkVertexBuffer.hpp)

################################################################################
# Set C++14 Standard as required.                                              #
################################################################################

SET_PROPERTY(TARGET SoVk PROPERTY CXX_STANDARD 14)
SET_PROPERTY(TARGET SoVk PROPERTY CXX_STANDARD_REQUIRED ON)

################################################################################
# Link with necessary libraries.                                               #
################################################################################

TARGET_LINK_LIBRARIES(SoVk ${Vulkan_LIBRARIES}
                           ${GLFW_LIBRARY}
                           ${assimp_LIBRARIES}
                           SoUtils)
