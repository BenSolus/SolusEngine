
###############################################################################
# Find packages                                                               #
###############################################################################

IF(CMAKE_COMPILER_IS_GNUCC)
  IF((CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8.0) OR FORCE_CXX11 OR FORCE_CXX14)
    FIND_PACKAGE(Boost COMPONENTS filesystem REQUIRED) 
  ENDIF()
ELSEIF(CMAKE_COMPILER_IS_CLANG)
  IF(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0 OR FORCE_CXX11 OR FORCE_CXX14)
    FIND_PACKAGE(Boost COMPONENTS filesystem REQUIRED)
  ENDIF()
ENDIF()

###############################################################################
# Assemble library.                                                           #
###############################################################################

FILE(GLOB ALL_SOURCES "*.hpp" "*.cpp")

SET(STD_SOURCES   "")
SET(BOOST_SOURCES "")

IF(CMAKE_COMPILER_IS_GNUCC)
  IF((CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8.0) OR FORCE_CXX11 OR FORCE_CXX14)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_BOOST_FILESYSTEM")
    FILE(GLOB BOOST_SOURCES "boost/*.hpp")
  ELSE()
    FILE(GLOB STD_SOURCES "std/*.hpp" "std/*.cpp")
  ENDIF()
ELSEIF(CMAKE_COMPILER_IS_CLANG)
  IF((CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0) OR FORCE_CXX11 OR FORCE_CXX14)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_BOOST_FILESYSTEM")
    FILE(GLOB BOOST_SOURCES "boost/*.hpp")
  ELSE()
    FILE(GLOB STD_SOURCES "std/*.hpp" "std/*.cpp")
  ENDIF()
ENDIF()

LIST(APPEND ALL_SOURCES ${BOOST_SOURCES} ${STD_SOURCES})

ADD_LIBRARY(SoCxx SHARED ${ALL_SOURCES})

###############################################################################
# Set various target properties.                                              #
###############################################################################

SET_HIGHEST_CXX_STANDARD(SoCxx)

###############################################################################
# Adding various target specific include directories.                         #
###############################################################################

TARGET_INCLUDE_DIRECTORIES(SoCxx PRIVATE ${PROJECT_SOURCE_DIR}/src/cxx)

IF(CMAKE_COMPILER_IS_GNUCC)
  IF((CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8.0) OR FORCE_CXX11 OR FORCE_CXX14) 
    TARGET_INCLUDE_DIRECTORIES(SoCxx PRIVATE ${Boost_INCLUDE_DIRS})   
  ENDIF()
ELSEIF(CMAKE_COMPILER_IS_CLANG)
  IF((CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0) OR FORCE_CXX11 OR FORCE_CXX14) 
    TARGET_INCLUDE_DIRECTORIES(SoCxx PRIVATE ${Boost_INCLUDE_DIRS}) 
  ENDIF()
ENDIF()

###############################################################################
# Link with necessary libraries.                                              #
###############################################################################

TARGET_LINK_LIBRARIES(SoCxx ${CMAKE_DL_LIBS})

IF(CMAKE_COMPILER_IS_GNUCC)
  IF((CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8.0) OR FORCE_CXX11 OR FORCE_CXX14) 
   TARGET_LINK_LIBRARIES(SoCxx ${Boost_LIBRARIES})
  ELSE()
   TARGET_LINK_LIBRARIES(SoCxx stdc++fs)  
  ENDIF()
ELSEIF(CMAKE_COMPILER_IS_CLANG)
  IF((CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0) OR FORCE_CXX11 OR FORCE_CXX14) 
    TARGET_LINK_LIBRARIES(SoCxx ${Boost_LIBRARIES})
  ELSE()
    TARGET_LINK_LIBRARIES(SoCxx stdc++fs) 
  ENDIF()
ENDIF()

