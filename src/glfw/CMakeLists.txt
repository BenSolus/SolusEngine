

###############################################################################
# Find necessary packages.                                                    #
###############################################################################

FIND_PACKAGE(GLFW REQUIRED)

###############################################################################
# Assemble library.                                                           #
###############################################################################

FILE(GLOB ALL_SOURCES "${PROJECT_SOURCE_DIR}/src/glfw/soGLFWBaseSurface.cpp"
                      "${PROJECT_SOURCE_DIR}/src/glfw/soGLFWBaseSurface.hpp"
                      "${PROJECT_SOURCE_DIR}/src/glfw/soGLFWSurface.cpp"
                      "${PROJECT_SOURCE_DIR}/src/glfw/soGLFWSurface.h")
 
FILE(GLOB CHILDREN
     RELATIVE
     ${PROJECT_SOURCE_DIR}/src/glfw
     ${PROJECT_SOURCE_DIR}/src/glfw/*)

FOREACH(CHILD ${CHILDREN})
  IF(IS_DIRECTORY "${PROJECT_SOURCE_DIR}/src/glfw/${CHILD}")

    FILE(GLOB
         PER_BACKEND_SOURCES
         "${PROJECT_SOURCE_DIR}/src/glfw/${CHILD}/*.hpp"
         "${PROJECT_SOURCE_DIR}/src/glfw/${CHILD}/*.cpp")

    LIST(APPEND ALL_SOURCES ${PER_BACKEND_SOURCES})
  ENDIF()
ENDFOREACH()

ADD_LIBRARY(SoGLFW SHARED ${ALL_SOURCES})

##############################################################################
# Set various target properties.                                              #
###############################################################################

SET_HIGHEST_CXX_STANDARD(SoGLFW)

###############################################################################
# Adding various target specific include directories.                         #
###############################################################################

TARGET_INCLUDE_DIRECTORIES(SoGLFW PUBLIC  ${GLFW_INCLUDE_DIR})
TARGET_INCLUDE_DIRECTORIES(SoGLFW PUBLIC  ${PROJECT_SOURCE_DIR}/src/cxx)
TARGET_INCLUDE_DIRECTORIES(SoGLFW PRIVATE ${PROJECT_SOURCE_DIR}/src/glfw)
TARGET_INCLUDE_DIRECTORIES(SoGLFW PRIVATE ${PROJECT_SOURCE_DIR}/src/glfw/vk)

###############################################################################
# Link with necessary libraries.                                              #
###############################################################################

TARGET_LINK_LIBRARIES(SoGLFW ${GLFW_LIBRARY})

###############################################################################
# Copy configuration files to binary directory.                               #
###############################################################################

FILE(GLOB CHILDREN 
     RELATIVE
     ${PROJECT_SOURCE_DIR}/src/glfw
     ${PROJECT_SOURCE_DIR}/src/glfw/*)

FOREACH(CHILD ${CHILDREN})
  IF(CHILD MATCHES "^(.+)\\.json$")
    STRING(REGEX REPLACE ".json" "" PROVIDED_BACKEND ${CHILD})
 
    SET(OUT_DIRECTORY
        "${PROJECT_BINARY_DIR}/data/backends/${PROVIDED_BACKEND}")

    IF(NOT IS_DIRECTORY ${OUT_DIRECTORY})
      FILE(MAKE_DIRECTORY ${OUT_DIRECTORY})
    ENDIF()

    ADD_CUSTOM_COMMAND(TARGET SoGLFW
                       POST_BUILD
                       COMMAND ${CMAKE_COMMAND}
                               -E
                               copy
                               ${PROJECT_SOURCE_DIR}/src/glfw/${CHILD}
                               ${OUT_DIRECTORY}/glfw.json)
  ENDIF()
ENDFOREACH()

