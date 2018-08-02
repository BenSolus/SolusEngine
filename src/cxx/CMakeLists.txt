
###############################################################################
# Assemble library.                                                           #
###############################################################################

FILE(GLOB CXX_SOURCES "*.hpp" "*.cpp")

ADD_LIBRARY(SoCxx SHARED ${CXX_SOURCES})

TARGET_LINK_LIBRARIES(SoCxx ${CMAKE_DL_LIBS})

TARGET_INCLUDE_DIRECTORIES(SoCxx PRIVATE ${PROJECT_SOURCE_DIR}/src/cxx)

###############################################################################
# Set highest available C++ Standard. Minimum C++11.                          #
###############################################################################

IF (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") OR
    ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"))
    CHECK_CXX_COMPILER_FLAG("-std=c++17" CXX_STANDARD)
ELSE()
  MESSAGE(FATAL_ERROR Compiler not supported.)
ENDIF()

IF (CXX_STANDARD)
  SET_PROPERTY(TARGET SoCxx PROPERTY CXX_STANDARD 17)
ELSE()
  IF (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") OR
      ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"))
    CHECK_CXX_COMPILER_FLAG("-std=c++14" CXX_STANDARD)
  ENDIF()

  IF (CXX_STANDARD)
    SET_PROPERTY(TARGET SoCxx PROPERTY CXX_STANDARD 14)
  ELSE()
    SET_PROPERTY(TARGET SoCxx PROPERTY CXX_STANDARD 11)
    SET_PROPERTY(TARGET SoCxx PROPERTY CXX_STANDARD_REQUIRED ON)
  ENDIF() 
ENDIF()
