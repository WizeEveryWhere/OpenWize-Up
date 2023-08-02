# adjust some ...
set(EXTEND_MOD Board_WizeUp)
set(BOARD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sources/board")
set(LD_SCRIPT STM32L451CEUX_def.ld)

set(BOOTSTRAP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sources/bootstrap")
#set(EXTEND_DIR "${BOOTSTRAP_DIR}/img")
set(EXTEND_DIR "${BOOTSTRAP_DIR}/img")

target_sources(${EXTEND_MOD} 
    INTERFACE 
        ${EXTEND_DIR}/src/wrap_system.c
    )
target_include_directories(${EXTEND_MOD}
    INTERFACE
        ${EXTEND_DIR}/include
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
#######