
###############################################################################
# Assemble library.                                                           #
###############################################################################

FILE(GLOB ALL_SOURCES "*.hpp" "*.cpp")

IF(CXX17_FOUND)
  FILE(GLOB STD_BACKEND_SOURCES "std/*.hpp" "std/*.cpp")

  LIST(APPEND ALL_SOURCES ${STD_BACKEND_SOURCES})
ENDIF()

ADD_LIBRARY(SoCxx SHARED ${ALL_SOURCES})

###############################################################################
# Set various target properties.                                              #
###############################################################################

SET_HIGHEST_CXX_STANDARD(SoCxx)

###############################################################################
# Adding various target specific include directories.                         #
###############################################################################

TARGET_INCLUDE_DIRECTORIES(SoCxx PRIVATE ${PROJECT_SOURCE_DIR}/src/cxx)

###############################################################################
# Link with necessary libraries.                                              #
###############################################################################

TARGET_LINK_LIBRARIES(SoCxx ${CMAKE_DL_LIBS})

IF(CXX17_FOUND)
  IF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANG)
	  TARGET_LINK_LIBRARIES(SoCxx stdc++fs) 
  ENDIF()
ENDIF()

