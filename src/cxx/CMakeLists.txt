
###############################################################################
# Find packages                                                               #
###############################################################################

IF(NOT CXX17_FOUND)
  FIND_PACKAGE(Boost COMPONENTS filesystem REQUIRED)
ENDIF()

###############################################################################
# Assemble library.                                                           #
###############################################################################

FILE(GLOB ALL_SOURCES "*.hpp" "*.cpp")

SET(BOOST_SOURCES "")

IF(CXX17_FOUND)
  FILE(GLOB STD_SOURCES "std/*.hpp" "std/*.cpp")

  LIST(APPEND ALL_SOURCES ${STD_SOURCES})

ELSE()
  FILE(GLOB BOOST_SOURCES "boost/*.hpp")
ENDIF()

LIST(APPEND ALL_SOURCES ${BOOST_SOURCES})

ADD_LIBRARY(SoCxx SHARED ${ALL_SOURCES})

###############################################################################
# Set various target properties.                                              #
###############################################################################

SET_HIGHEST_CXX_STANDARD(SoCxx)

###############################################################################
# Adding various target specific include directories.                         #
###############################################################################

TARGET_INCLUDE_DIRECTORIES(SoCxx PRIVATE ${PROJECT_SOURCE_DIR}/src/cxx)

IF(NOT CXX17_FOUND)
  TARGET_INCLUDE_DIRECTORIES(SoCxx PRIVATE ${Boost_INCLUDE_DIRS})
ENDIF()

###############################################################################
# Link with necessary libraries.                                              #
###############################################################################

TARGET_LINK_LIBRARIES(SoCxx ${CMAKE_DL_LIBS})

IF(CXX17_FOUND)
  IF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANG)
	  TARGET_LINK_LIBRARIES(SoCxx stdc++fs) 
  ENDIF()
ELSE()
  TARGET_LINK_LIBRARIES(SoCxx ${Boost_LIBRARIES})
ENDIF()

