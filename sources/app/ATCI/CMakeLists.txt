################################################################################

set(MODULE_NAME atci)

################################################################################

add_library(${MODULE_NAME} INTERFACE)

# Add sources to Build
target_sources(${MODULE_NAME}
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/atci.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/atci_get_cmd.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/atci_resp.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/console.c
    )

# Add include dir    
target_include_directories(
    ${MODULE_NAME} 
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

# Add dependencies
target_link_libraries(${MODULE_NAME} 
    INTERFACE 
        bsp 
        openwize
    )
