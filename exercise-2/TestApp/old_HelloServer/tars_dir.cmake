project(${PROJ_NAME})

# set tars install dir
IF(NOT DEFINED TARS_INSTALL_DIR)
    set(TARS_INSTALL_DIR "/usr/local/tars/cpp")
ELSE()                                                                                                                                                                   
    IF(NOT IS_ABSOLUTE ${TARS_INSTALL_DIR})                                                                                                                                                                                                                                                                
        set(TARS_INSTALL_DIR "${CMAKE_BINARY_DIR}/${TARS_INSTALL_DIR}")                                                                                                                                                                                                                       
    ENDIF() 
ENDIF()
