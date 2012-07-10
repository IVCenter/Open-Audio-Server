CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Set the build type
SET(OAS_DEFAULT_CMAKE_BUILD_TYPE RelWithDebInfo)

IF(DEFINED CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel")
ELSE()
   SET(CMAKE_BUILD_TYPE ${OAS_DEFAULT_CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel.")
ENDIF()

# Start defining the project
PROJECT (OAS)

# Set version numbers
SET(OAS_MAJOR_VERSION 0)
SET(OAS_MINOR_VERSION 91)
SET(OAS_VERSION "${OAS_MAJOR_VERSION}.${OAS_MINOR_VERSION}")

# Set cmake module path
SET(CMAKE_MODULE_PATH "${OAS_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

# Set the location of the binary output directory
SET(EXECUTABLE_OUTPUT_PATH "${OAS_SOURCE_DIR}/build/bin")

# Set the location of input files that CMake should configure
SET(OAS_CMAKEFILES_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

OPTION(BUILD_GUI "Look for FLTK library, to build GUI components of server" ON)

# Find relevant packages
FIND_PACKAGE(OPENAL REQUIRED)
FIND_PACKAGE(ALUT REQUIRED)
FIND_PACKAGE(MXML REQUIRED)

IF(BUILD_GUI)
    SET(FLTK_SKIP_FORMS TRUE)
    SET(FLTK_SKIP_IMAGES TRUE)
    SET(FLTK_SKIP_FLUID TRUE)
    FIND_PACKAGE(FLTK 1.3.0)
ENDIF(BUILD_GUI)

FIND_PACKAGE(DOXYGEN)

IF(DOXYGEN_FOUND)
    SET(OAS_DOCUMENTATION_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs)

    CONFIGURE_FILE(${OAS_CMAKEFILES_DIRECTORY}/Doxyfile.in
        ${OAS_DOCUMENTATION_DIRECTORY}/Doxyfile @ONLY
    )

    ADD_CUSTOM_TARGET(docs ${DOXYGEN_EXECUTABLE}
        ${OAS_DOCUMENTATION_DIRECTORY}/Doxyfile 
        WORKING_DIRECTORY ${OAS_DOCUMENTATION_DIRECTORY}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
ENDIF(DOXYGEN_FOUND)

# Generate the config file, to resolve dependencies at compile-time
CONFIGURE_FILE(${OAS_CMAKEFILES_DIRECTORY}/config.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h)

IF(FLTK_FOUND)
    ADD_EXECUTABLE(OAS
        src/OASServer.cpp 
        src/OASSocketHandler.cpp
        src/OASAudioHandler.cpp 
        src/OASAudioBuffer.cpp 
        src/OASAudioSource.cpp 
        src/OASAudioListener.cpp 
        src/OASLogger.cpp 
        src/OASServerWindow.cpp 
        src/OASFileHandler.cpp 
        src/OASMessage.cpp 
        src/OASServerInfo.cpp 
        src/OASServerWindowLogBrowser.cpp 
        src/OASServerWindowTable.cpp
        src/OASTime.cpp)
ELSE(FLTK_FOUND)
    ADD_EXECUTABLE(OAS
        src/OASServer.cpp 
        src/OASSocketHandler.cpp
        src/OASAudioHandler.cpp 
        src/OASAudioBuffer.cpp 
        src/OASAudioSource.cpp 
        src/OASAudioListener.cpp 
        src/OASLogger.cpp 
        src/OASFileHandler.cpp 
        src/OASMessage.cpp 
        src/OASServerInfo.cpp 
        src/OASTime.cpp)
ENDIF(FLTK_FOUND)



INCLUDE_DIRECTORIES(src)
INCLUDE_DIRECTORIES(${OPENAL_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${ALUT_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${FLTK_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${MXML_INCLUDE_DIR})

TARGET_LINK_LIBRARIES(OAS ${OPENAL_LIBRARY})
TARGET_LINK_LIBRARIES(OAS ${ALUT_LIBRARY})
TARGET_LINK_LIBRARIES(OAS ${FLTK_LIBRARY})
TARGET_LINK_LIBRARIES(OAS ${MXML_LIBRARY})
TARGET_LINK_LIBRARIES(OAS rt)

INSTALL(TARGETS OAS DESTINATION bin)


