################################################################################

set(MODULE_NAME adf7030)

################################################################################

add_library(${MODULE_NAME} STATIC)

# Add sources to Build
target_sources(${MODULE_NAME}
    PRIVATE
        src/adf7030-1_phy.c
        src/adf7030-1_phy_conv.c
        src/adf7030-1_phy_log.c
        src/phy_layer.c
        adf7030-1/src/adf7030-1__cfg.c
        adf7030-1/src/adf7030-1__gpio.c
        adf7030-1/src/adf7030-1__irq.c
        adf7030-1/src/adf7030-1__mem.c
        adf7030-1/src/adf7030-1__patch.c
        adf7030-1/src/adf7030-1__spi.c
        adf7030-1/src/adf7030-1__state.c
        adf7030-1/src/adf7030-1__trig.c
    )

# Add include dir    
target_include_directories(
    ${MODULE_NAME} 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/conf
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/adf7030-1/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

# Add dependencies
target_link_libraries(${MODULE_NAME} 
    PUBLIC 
        bsp 
        openwize
    )
