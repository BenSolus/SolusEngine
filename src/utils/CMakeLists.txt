
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/utils/def)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/utils/err)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/utils/fs)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/utils/mem)

################################################################################
# Add several utility functions.                                               #
################################################################################

ADD_LIBRARY(SoUtils SHARED def/soDefinitions.hpp
                           err/soException.hpp
                           fs/soFileSystem.cpp
                           fs/soFileSystem.hpp
                           mem/soAlignedAllocator.hpp
                           mem/soAlignedVector.hpp)

################################################################################
# Set C++14 Standard as required.                                              #
################################################################################

SET_PROPERTY(TARGET SoUtils PROPERTY CXX_STANDARD 14)
SET_PROPERTY(TARGET SoUtils PROPERTY CXX_STANDARD_REQUIRED ON)