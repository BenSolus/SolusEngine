
###############################################################################
# Assemble library.                                                           #
###############################################################################

FILE(GLOB sources "*.hpp" "*.cpp")

ADD_LIBRARY(SoVk SHARED ${sources})

##############################################################################
# Set various target properties.                                              #
###############################################################################

SET_HIGHEST_CXX_STANDARD(SoVk)

###############################################################################
# Adding various target specific include directories.                         #
###############################################################################

TARGET_INCLUDE_DIRECTORIES(SoVk PUBLIC  ${PROJECT_SOURCE_DIR}/src/cxx)
TARGET_INCLUDE_DIRECTORIES(SoVk PRIVATE ${PROJECT_SOURCE_DIR}/src/vk)

