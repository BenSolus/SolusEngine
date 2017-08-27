CMAKE_MINIMUM_REQUIRED(VERSION 3.2.2)

PROJECT(vulkan-engine)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
  ENABLE_TESTING()
ENDIF()

################################################################################
# Setup for various paths                                                      #
################################################################################

SET(CMAKE_MODULE_PATH      "${PROJECT_SOURCE_DIR}/moduls")
SET(LIBRARY_OUTPUT_PATH    "${PROJECT_BINARY_DIR}/lib")
SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

################################################################################
# Find packages                                                                #
################################################################################

FIND_PACKAGE(assimp REQUIRED)
FIND_PACKAGE(GLFW REQUIRED)
FIND_PACKAGE(GLM REQUIRED)
FIND_PACKAGE(Vulkan REQUIRED)
FIND_PACKAGE(VulkanSDK REQUIRED)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
  FIND_PACKAGE(GTest REQUIRED)
ENDIF()

################################################################################
# Adding various include directories                                           #
################################################################################

INCLUDE_DIRECTORIES(SYSTEM ${PROJECT_SOURCE_DIR}/ext/stb)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)

INCLUDE_DIRECTORIES(${assimp_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${GLFW_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${GLM_REQUIRED})
INCLUDE_DIRECTORIES(${Vulkan_INCLUDE_DIRS})

IF(CMAKE_BUILD_TYPE MATCHES Debug)
  INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/tests)

  INCLUDE_DIRECTORIES(${GTEST_INCLUDE_DIRS})
ENDIF()

################################################################################
# Default compiler flags                                                       #
################################################################################

SET(DEFAULT_CXX_FLAGS "-Wall -Wextra -pedantic ")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DEFAULT_CXX_FLAGS}")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wcast-align -Wconversion -Weffc++ -Wfloat-equal -Wformat=2 -Wformat-nonliteral -Winvalid-pch -Wold-style-cast -Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wstrict-overflow=5 -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -DCMAKE_BIN_DIR=\\\"${PROJECT_BINARY_DIR}\\\"")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

IF(CMAKE_COMPILER_IS_GNUCC)
  EXECUTE_PROCESS(COMMAND ${CMAKE_C_COMPILER} -dumpversion
                  OUTPUT_VARIABLE GCC_VERSION)
  IF(GCC_VERSION VERSION_LESS 7.1.1)
    # Some older version of gcc spams
    # 'warning: missing initializer for member ...' warnings. Don't know in
    # which version they 'fixed' this.
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-field-initializers")
  ENDIF()
ENDIF()

################################################################################
# Add source subdirectory                                                      #
################################################################################

ADD_SUBDIRECTORY(src)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
  ADD_SUBDIRECTORY(tests)
ENDIF()

ADD_EXECUTABLE(main main.cpp)

TARGET_LINK_LIBRARIES(main SoEng)

SET_PROPERTY(TARGET main PROPERTY CXX_STANDARD 14)
SET_PROPERTY(TARGET main PROPERTY CXX_STANDARD_REQUIRED ON)