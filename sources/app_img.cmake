# adjust some ...
set(EXTEND_MOD Board_WizeUp)
set(BOARD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sources/board")

set(BOOTSTRAP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sources/bootstrap")
set(EXTEND_DIR "${BOOTSTRAP_DIR}/img")

#######
if(NOT BUILD_STANDALONE_APP)
#else()

set(LD_SCRIPT STM32L451CEUX_def.ld)

target_sources(${EXTEND_MOD} 
    INTERFACE 
        ${EXTEND_DIR}/src/wrap_system.c
    )

set_property( 
    TARGET ${EXTEND_MOD}
    PROPERTY 
        INTERFACE_LINK_OPTIONS 
        "-Wl,--wrap=SystemInit"
        "-T${BOARD_DIR}/ld/${LD_SCRIPT}"
        "-T${EXTEND_DIR}/ld/definition.ld"
        "-T${BOARD_DIR}/ld/sections.ld"
    )

endif()

#######
target_include_directories(${EXTEND_MOD}
    INTERFACE
        ${EXTEND_DIR}/include
    )

#######
set(BSP_EXTEND_MOD bsp)
target_include_directories(${BSP_EXTEND_MOD}
    PUBLIC
        ${EXTEND_DIR}/include
    )
#######