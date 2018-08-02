
###############################################################################
# Add public interface.                                                       #
###############################################################################

FILE(GLOB sources "*.hpp" "*.cpp")
FILE(GLOB interfaces "interfaces/*.hpp")
FILE(GLOB glfw "glfw/*.hpp")

ADD_LIBRARY(SoVk SHARED ${sources} ${interfaces} ${glfw})

TARGET_INCLUDE_DIRECTORIES(SoVk PUBLIC ${PROJECT_SOURCE_DIR}/src/cxx)
TARGET_INCLUDE_DIRECTORIES(SoVk PUBLIC ${PROJECT_SOURCE_DIR}/src/vk)

###############################################################################
# Set C++14 Standard as required.                                             #
###############################################################################

SET_PROPERTY(TARGET SoVk PROPERTY LINKER_LANGUAGE CXX)
SET_PROPERTY(TARGET SoVk PROPERTY CXX_STANDARD 14)
SET_PROPERTY(TARGET SoVk PROPERTY CXX_STANDARD_REQUIRED ON)

###############################################################################
# Link with necessary libraries.                                              #
###############################################################################

TARGET_LINK_LIBRARIES(SoVk ${GLFW_LIBRARY})

