
# Define a macro to create and link examples
MACRO(ADD_EXAMPLE E_NAME E_EXEC E_SOURCE)
    OPTION(EXAMPLES_${E_NAME} "Build the ${E_EXEC} example" ON)

    IF(EXAMPLES_${E_NAME})
        ADD_EXECUTABLE(${E_EXEC} ${E_SOURCE})
        TARGET_LINK_LIBRARIES(${E_EXEC} ${OASCLIENT_LIBRARY_NAME})
    ENDIF(EXAMPLES_${E_NAME})
ENDMACRO(ADD_EXAMPLE)

# Add examples here
ADD_EXAMPLE(TEST test test.cpp)
ADD_EXAMPLE(DOPPLER doppler doppler.cpp)

