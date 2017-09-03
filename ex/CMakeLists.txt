
################################################################################
# Add all example folders                                                      #
################################################################################

FILE(GLOB
     CONTENT
     RELATIVE
     "${PROJECT_SOURCE_DIR}/ex"
     "${PROJECT_SOURCE_DIR}/ex/*")

FOREACH(ITEM ${CONTENT})
  IF(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/ex/${ITEM})
    ADD_SUBDIRECTORY(${ITEM})
  ENDIF()
ENDFOREACH()