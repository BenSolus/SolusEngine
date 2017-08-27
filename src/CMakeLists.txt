
################################################################################
# Add source subdirectory.                                                     #
################################################################################

ADD_SUBDIRECTORY(vk)
ADD_SUBDIRECTORY(utils)

################################################################################
# Add public interface.                                                        #
################################################################################

ADD_LIBRARY(SoEng SHARED soEngine.cpp soEngine.hpp)

################################################################################
# Set C++14 Standard as required.                                              #
################################################################################

SET_PROPERTY(TARGET SoEng PROPERTY CXX_STANDARD 14)
SET_PROPERTY(TARGET SoEng PROPERTY CXX_STANDARD_REQUIRED ON)

################################################################################
# Link with necessary libraries.                                               #
################################################################################

TARGET_LINK_LIBRARIES(SoEng SoVk)

################################################################################
# Compile shaders.                                                             #
################################################################################

IF(NOT IS_DIRECTORY "${PROJECT_BINARY_DIR}/data/shaders")
  FILE(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/data/shaders")
ENDIF()

ADD_CUSTOM_COMMAND(TARGET SoEng POST_BUILD
                   COMMAND ${GLSLANGVALIDATOR} -V "${PROJECT_SOURCE_DIR}/src/shaders/*"
                   WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/data/shaders")