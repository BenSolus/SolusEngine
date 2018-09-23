
###############################################################################
# Add source subdirectories which SoEng will depend on.                       #
###############################################################################

ADD_SUBDIRECTORY(cxx)

IF(WITH_GLFW)
  ADD_SUBDIRECTORY(glfw)
ENDIF()

IF(WITH_VULKAN)
  ADD_SUBDIRECTORY(vk)
ENDIF()

###############################################################################
# Assemble library.                                                           #
###############################################################################

FILE(GLOB interfaces "interfaces/*.hpp")

ADD_LIBRARY(SoEng SHARED soEngine.cpp soEngine.h ${interfaces})

###############################################################################
# Set various target properties.                                              #
###############################################################################

SET_HIGHEST_CXX_STANDARD(SoEng)

###############################################################################
# Adding various target specific include directories.                         #
###############################################################################

TARGET_INCLUDE_DIRECTORIES(SoEng PUBLIC ${PROJECT_SOURCE_DIR}/src/vk)

###############################################################################
# Link with necessary libraries.                                              #
###############################################################################

TARGET_LINK_LIBRARIES(SoEng SoVk SoCxx)

###############################################################################
# Compile shaders.                                                            #
###############################################################################

FIND_PACKAGE(VulkanSDK REQUIRED)

IF(NOT IS_DIRECTORY "${PROJECT_BINARY_DIR}/data/shaders")
  FILE(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/data/shaders")
ENDIF()

FILE(GLOB children
     RELATIVE
     ${PROJECT_SOURCE_DIR}/src/shaders
     ${PROJECT_SOURCE_DIR}/src/shaders/*)

FOREACH(child ${children})
  IF(IS_DIRECTORY "${PROJECT_SOURCE_DIR}/src/shaders/${child}")
    IF(NOT IS_DIRECTORY "${PROJECT_BINARY_DIR}/data/shaders/${child}") 
      FILE(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/data/shaders/${child}")
    ENDIF()

    ADD_CUSTOM_COMMAND(TARGET SoEng
                       POST_BUILD
                       COMMAND ${GLSLANGVALIDATOR}
                               -V
                               "${PROJECT_SOURCE_DIR}/src/shaders/${child}/*"
                       WORKING_DIRECTORY
                       "${PROJECT_BINARY_DIR}/data/shaders/${child}")
  ENDIF()
ENDFOREACH()

###############################################################################
# Copy compile_commands.json to root directory.                               #
###############################################################################

IF(CMAKE_EXPORT_COMPILE_COMMANDS EQUAL 1)
  ADD_CUSTOM_COMMAND(TARGET SoEng
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy
                     ${PROJECT_BINARY_DIR}/compile_commands.json
                     ${PROJECT_SOURCE_DIR})
ENDIF()

