
###############################################################################
# Add directories to include.                                                 #
###############################################################################

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/vk)

###############################################################################
# Add source subdirectory.                                                    #
###############################################################################

ADD_SUBDIRECTORY(vk)
ADD_SUBDIRECTORY(utils)

###############################################################################
# Add public interface.                                                       #
###############################################################################

FILE(GLOB interfaces "interfaces/*.hpp")

ADD_LIBRARY(SoEng SHARED soEngine.hpp ${interfaces})

###############################################################################
# Set C++14 Standard as required.                                             #
###############################################################################

SET_PROPERTY(TARGET SoEng PROPERTY LINKER_LANGUAGE CXX)
SET_PROPERTY(TARGET SoEng PROPERTY CXX_STANDARD 14)
SET_PROPERTY(TARGET SoEng PROPERTY CXX_STANDARD_REQUIRED ON)

###############################################################################
# Link with necessary libraries.                                              #
###############################################################################

TARGET_LINK_LIBRARIES(SoEng SoVk)

###############################################################################
# Compile shaders.                                                            #
###############################################################################

IF(NOT IS_DIRECTORY "${PROJECT_BINARY_DIR}/data/shaders")
  FILE(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/data/shaders")
ENDIF()

ADD_CUSTOM_COMMAND(TARGET SoEng POST_BUILD
                   COMMAND ${GLSLANGVALIDATOR} -V "${PROJECT_SOURCE_DIR}/src/shaders/*"
                   WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/data/shaders")

###############################################################################
# Copy compile_commands.json to root directory.                               #
###############################################################################

IF(CMAKE_EXPORT_COMPILE_COMMANDS EQUAL 1)
  ADD_CUSTOM_COMMAND(TARGET SoEng
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND}
                     ${PROJECT_BINARY_DIR}/compile_commands.json
                     ${PROJECT_SOURCE_DIR})
ENDIF()
