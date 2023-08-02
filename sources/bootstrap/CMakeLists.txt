################################################################################
# include the build support cmake
find_package(build_support REQUIRED)

set(MODULE_NAME bootstrap)
set(LD_SCRIPT STM32L451CEUX_def.ld)
set(STARTUP   startup_stm32l4xx.s)

################################################################################
option(HAS_2ND_STAGE_OR_SYS_BL "TODO : description." OFF)
option(HAS_CRC_COMPUTE         "Compute CRC for exchange area" ON)
option(HAS_SHA256_COMPUTE      "TODO : description." OFF)
option(HAS_AES256_COMPUTE      "TODO : description." OFF)
option(USE_BOOTSTRAP_TRACE     "Activate the trace." ON)
option(USE_LPUART_COM          "Use the LPUART as trace port (default UART4)" OFF)

################################################################################

# Add executable 
add_executable(${MODULE_NAME})

# Add sources to Build
target_sources(${MODULE_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/bootstrap.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/preload.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/exception.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/swap.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/flash.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/uart.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/trace.c
    	${CMAKE_CURRENT_SOURCE_DIR}/startup/${STARTUP}
    )

# Add include dir    
target_include_directories(
    ${MODULE_NAME} 
    PUBLIC
    	${CMAKE_CURRENT_SOURCE_DIR}/include
    	${CMAKE_CURRENT_SOURCE_DIR}/img/include
    )
    
# Add some link flags
target_link_options(${MODULE_NAME}
    PUBLIC
        --specs=nano.specs 
        -Wl,-Map=${MODULE_NAME}.map 
        -Wl,--gc-sections
        "-T${CMAKE_CURRENT_SOURCE_DIR}/ld/${LD_SCRIPT}"
        "-T${CMAKE_CURRENT_SOURCE_DIR}/ld/definition.ld"
        "-T${CMAKE_CURRENT_SOURCE_DIR}/ld/alias.ld"
        "-T${CMAKE_CURRENT_SOURCE_DIR}/ld/boot.ld"
    )
    
if(HAS_2ND_STAGE_OR_SYS_BL)
	add_compile_definitions(HAS_2ND_STAGE_OR_SYS_BL=1)
endif(HAS_2ND_STAGE_OR_SYS_BL)


if(HAS_CRC_COMPUTE)
    set(HAL_CRC_MODULE_ENABLE ON)
    target_sources(${MODULE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/crc.c)
    add_compile_definitions(HAS_CRC_COMPUTE=1)
endif(HAS_CRC_COMPUTE)

if(HAS_SHA256_COMPUTE OR HAS_AES256_COMPUTE)
    if(HAS_SHA256_COMPUTE)
        add_compile_definitions(HAS_SHA256_COMPUTE=1)
    endif(HAS_SHA256_COMPUTE)
    
    if(HAS_AES256_COMPUTE)
        add_compile_definitions(HAS_AES256_COMPUTE=1)
    endif(HAS_AES256_COMPUTE)
    
    set(BUILD_TINYCRYPT ON)
    target_link_libraries(${MODULE_NAME} PUBLIC 3rd::tinycrypt)
endif(HAS_SHA256_COMPUTE OR HAS_AES256_COMPUTE)

if(USE_BOOTSTRAP_TRACE)
    add_compile_definitions(USE_BOOTSTRAP_TRACE=1)
endif(USE_BOOTSTRAP_TRACE)

if(USE_LPUART_COM)
    add_compile_definitions(USE_LPUART1=1)
endif(USE_LPUART_COM)

################################################################################
setup_install(
    TARGET ${MODULE_NAME} 
    DEPENDS
    	3rd::stm32l4xx_cmsis
        3rd::stm32l4xx_hal
        3rd::stm32l4xx_ll
    )
