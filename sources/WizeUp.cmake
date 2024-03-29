cmake_minimum_required( VERSION 3.12 ) 
################################################################################

set(BOARD_NAME "Alciom Wize'Up Board")
set(APP_NAME "OpenwWizeUp App")

add_compile_definitions(HAS_NO_BANNER=1)
add_compile_definitions(USE_RTOS_TRACE=1)
add_compile_definitions(HAS_BSP_PWRLINE=1)
add_compile_definitions(LOGGER_HAS_COLOR=1)
add_compile_definitions(LOGGER_USE_FWRITE=1)
add_compile_definitions(DUMP_CORE_HAS_TRACE=1)
add_compile_definitions(DUMP_CORE_HAS_FAULT_STATUS_REGISTER=1)
add_compile_definitions(L6VERS=L6VER_WIZE_REV_1_2)

add_compile_definitions(USE_SPI=1)
# add_compile_definitions(USE_I2C=1)

add_compile_definitions(CONSOLE_RX_TIMEOUT=5000)

# OpenWizeUp Options
include(sources/OpenWizeUp_Options.cmake)

add_compile_options(-Wall -ffunction-sections -fdata-sections -fstack-usage)

################################################################################
# Set Coss compile
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CROSSCOMPILING TRUE )

# Use FreeRTOS
set(USE_FREERTOS ON)

# Set MCU
set(VENDOR_MCU "STM32L451xx")

set(HAL_CONF_FILE "${CMAKE_CURRENT_SOURCE_DIR}/sources/STM32HALConfig.cmake")
set(RTOS_CONF_FILE "${CMAKE_CURRENT_SOURCE_DIR}/sources/FreeRTOSConfig.cmake")

################################################################################
find_package(cross_stm32 REQUIRED)
set_cross_stm32()

find_package(toolchain REQUIRED)

################################################################################
if(USE_FREERTOS)
    # Set port arch
    if(${MCU_ARCH} STREQUAL "cortex-m4")
        set(RTOS_PORT_NAME "ARM_CM4F")
    else()
        message(FATAL_ERROR "MCU_ARCH is not defined")
    endif()

    # Set port arch path
    set(RTOS_PORT_PATH "GCC/${RTOS_PORT_NAME}" )
endif(USE_FREERTOS)

################################################################################
set(opt 0)
if(BUILD_NVM_BINARY)
    set(opt 1)
endif(BUILD_NVM_BINARY)
# Generate parameters 
# (only if "-DGENERATE_PARAM=ON" is added on cmake command line)
find_package(gen_param REQUIRED)
gen_param(
    SOURCE ${PARAM_XML_FILE_LIST}
    DESTINATION sources/app
    OPT ${opt}
    )

################################################################################
## subdirectories
set(MOD_DIR_LST 
    "sources/bsp" 
    "sources/board" 
    "sources/device/FlashStorage" 
    "sources/device/Adf7030" 
    "sources/app" 
    )

# Add subdirectories
set(MAIN_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
foreach(mod_dir ${MOD_DIR_LST})
    message("     *** Add ${mod_dir} : ")
    set(tmp_src_dir "${MAIN_SRC_DIR}/${mod_dir}")
    if(EXISTS "${tmp_src_dir}")
        # Add Sources
        add_subdirectory("${tmp_src_dir}")
        # Add docs
        list(APPEND DOC_SOURCE_DIRS "${tmp_src_dir}")
    endif()
endforeach(mod_dir)

if(BUILD_STANDALAONE_APP)
else()
    include(sources/app_img.cmake)
endif(BUILD_STANDALAONE_APP)
