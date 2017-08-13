
################################################################################
# Add source subdirectory                                                      #
################################################################################

ADD_SUBDIRECTORY(cc)

ADD_LIBRARY(VkEng SHARED
            vkBuffer.cpp
            vkBuffer.hpp
            vkCommandBuffers.cpp
            vkCommandBuffers.hpp
            vkCommandPool.cpp
            vkCommandPool.hpp
            vkDebugReportCallbackEXT.cpp
            vkDebugReportCallbackEXT.hpp
            vkDepthImage.cpp
            vkDepthImage.hpp
            vkDescriptorPool.cpp
            vkDescriptorPool.hpp
            vkDescriptorSet.cpp
            vkDescriptorSet.hpp
            vkDescriptorSetLayout.cpp
            vkDescriptorSetLayout.hpp
            vkFramebuffers.cpp
            vkFramebuffers.hpp
            vkFramework.cpp
            vkFramework.hpp
            vkImage.cpp
            vkImage.hpp
            vkImageViews.cpp
            vkImageViews.hpp
            vkIndexBuffer.cpp
            vkIndexBuffer.hpp
            vkInstance.cpp
            vkInstance.hpp
            vkLogicalDevice.cpp
            vkLogicalDevice.hpp
            vkModel.cpp
            vkModel.hpp
            vkPhysicalDevice.cpp
            vkPhysicalDevice.hpp
            vkPipeline.cpp
            vkPipeline.hpp
            vkQueueFamilyIndices.cpp
            vkQueueFamilyIndices.hpp
            vkRenderPass.cpp
            vkRenderPass.hpp
            vkResource.hpp
            vkSemaphore.cpp
            vkSemaphore.hpp
            vkShaderModule.cpp
            vkShaderModule.hpp
            vkStagingBuffer.hpp
            vkStagingBuffer.cpp
            vkSurface.cpp
            vkSurface.hpp
            vkSwapChain.cpp
            vkSwapChain.hpp
            vkSwapChainSupportDetails.cpp
            vkSwapChainSupportDetails.hpp
            vkTextureImage.cpp
            vkTextureImage.hpp
            vkTextureSampler.cpp
            vkTextureSampler.hpp
            vkUniformBuffer.cpp
            vkUniformBuffer.hpp
            vkVertex.cpp
            vkVertex.hpp
            vkVertexBuffer.cpp
            vkVertexBuffer.hpp)

TARGET_LINK_LIBRARIES(VkEng ${Vulkan_LIBRARIES}
                            ${GLFW_LIBRARY}
                            ${assimp_LIBRARIES}
                            cc)

IF(NOT IS_DIRECTORY "${PROJECT_BINARY_DIR}/data/shaders")
    FILE(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/data/shaders")
ENDIF()

ADD_CUSTOM_COMMAND(TARGET VkEng POST_BUILD
                   COMMAND ${GLSLANGVALIDATOR} -V "${PROJECT_SOURCE_DIR}/src/shaders/*"
                   WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/data/shaders")