################################################################################
 
set(MODULE_NAME Board_WizeUp)
set(LD_SCRIPT STM32L451CEUX_def.ld)
set(STARTUP   startup_stm32l451ceux.s)

################################################################################
add_library(${MODULE_NAME} INTERFACE)

target_sources(${MODULE_NAME} 
    INTERFACE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/stm32l4xx_hal_msp.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/stm32l4xx_hal_timebase_tim.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/stm32l4xx_it.c
        ${CMAKE_CURRENT_SOURCE_DIR}/startup/${STARTUP}
    )
target_include_directories(${MODULE_NAME}
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

target_link_libraries(${MODULE_NAME}
    INTERFACE
        stm32l4xx_cmsis
        stm32l4xx_hal
        stm32l4xx_ll
    )

if (USE_SEMIHOSTING)
target_link_libraries(${MODULE_NAME}
    INTERFACE
        rdimon
    )
endif()

# add ldscript
set_property( 
    TARGET
        ${MODULE_NAME}
    PROPERTY 
        INTERFACE_LINK_OPTIONS 
        "-T${CMAKE_CURRENT_SOURCE_DIR}/ld/${LD_SCRIPT}"
        "-T${CMAKE_CURRENT_SOURCE_DIR}/ld/definition.ld"
        "-T${CMAKE_CURRENT_SOURCE_DIR}/ld/sections.ld"
    )
