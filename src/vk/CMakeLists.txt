
###############################################################################
# Assemble library.                                                           #
###############################################################################

FILE(GLOB sources "*.hpp" "*.cpp")
FILE(GLOB interfaces "interfaces/*.hpp")
FILE(GLOB glfw "glfw/*.hpp")

ADD_LIBRARY(SoVk SHARED ${sources} ${interfaces} ${glfw})

##############################################################################
# Set various target properties.                                              #
###############################################################################

SET_HIGHEST_CXX_STANDARD(SoVk)

###############################################################################
# Adding various target specific include directories.                         #
###############################################################################

TARGET_INCLUDE_DIRECTORIES(SoVk PUBLIC  ${PROJECT_SOURCE_DIR}/src/cxx)
TARGET_INCLUDE_DIRECTORIES(SoVk PRIVATE ${PROJECT_SOURCE_DIR}/src/vk)

###############################################################################
# Link with necessary libraries.                                              #
###############################################################################

TARGET_LINK_LIBRARIES(SoVk ${GLFW_LIBRARY})

