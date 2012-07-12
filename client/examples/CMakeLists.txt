
# Find the OASCLIENT package, using the provided FindOASCLIENT CMake Module
FIND_PACKAGE(OASCLIENT REQUIRED)

# If OAS was not found in the usual areas, try using the library and source
# that was provided and compiled by this repository
IF (NOT OASCLIENT_FOUND)
    FIND_LIBRARY(OASCLIENT_LIBRARY 
        NAMES oasclient liboasclient
        PATHS
            ${CMAKE_CURRENT_SOURCE_DIR}/../build/lib
            ${CMAKE_CURRENT_SOURCE_DIR}/../build/lib64
    )
    FIND_PATH(OASCLIENT_INCLUDE_DIR OASSound.h
        PATHS
            ${CMAKE_CURRENT_SOURCE_DIR}/../src
    )
ENDIF(NOT OASCLIENT_FOUND)

# Define a macro to create and link examples
MACRO(ADD_EXAMPLE E_NAME E_EXEC E_SOURCE)
    OPTION(EXAMPLES_${E_NAME} "Build the ${E_EXEC} example" OFF)

    IF(EXAMPLES_${E_NAME})
        ADD_EXECUTABLE(${E_EXEC} ${E_SOURCE})
        TARGET_LINK_LIBRARIES(${E_EXEC} ${OASCLIENT_LIBRARY})
        INCLUDE_DIRECTORIES(${OASCLIENT_INCLUDE_DIR})
    ENDIF(EXAMPLES_${E_NAME})
ENDMACRO(ADD_EXAMPLE)

# Add examples here
ADD_EXAMPLE(TEST test test.cpp)
ADD_EXAMPLE(DOPPLER doppler doppler.cpp)

