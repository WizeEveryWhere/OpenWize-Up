################################################################################

set(MODULE_NAME extra)

################################################################################

add_library(${MODULE_NAME} INTERFACE)

# Add sources to Build
target_sources(${MODULE_NAME}
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/simple_at/src/at_extend_cmd.c
        ${CMAKE_CURRENT_SOURCE_DIR}/simple_at/src/at_lo_update_cmd.c
        ${CMAKE_CURRENT_SOURCE_DIR}/simple_at/src/at_ext_update_cmd.c
        ${CMAKE_CURRENT_SOURCE_DIR}/simple_at/src/at_key_cmd.c
        ${CMAKE_CURRENT_SOURCE_DIR}/local_itf/src/local_itf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/phy_test/src/phy_test.c
    )

# Add include dir    
target_include_directories(
    ${MODULE_NAME} 
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/simple_at/include
        ${CMAKE_CURRENT_SOURCE_DIR}/local_itf/include
        ${CMAKE_CURRENT_SOURCE_DIR}/phy_test/include
    )

# Add dependencies
target_link_libraries(${MODULE_NAME} 
    INTERFACE 
        bsp 
        # atci
        # adf7030 
        openwize 
    )
