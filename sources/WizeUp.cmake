cmake_minimum_required( VERSION 3.12 ) 
################################################################################

set(BOARD_NAME "Alciom Wize'Up Board")
set(APP_NAME "OpenwWizeUp App")

add_compile_options(-Wall -ffunction-sections -fdata-sections -fstack-usage)

# OpenWizeUp Options
include(sources/OpenWizeUp_Options.cmake)

if(HAS_TRACE_FACILITY)
    # Add to global include
    include_directories(${CMAKE_SOURCE_DIR}/sources/trace_facility/include) 
endif(HAS_TRACE_FACILITY)

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
# Generate parameters 
#
# (only if "-DGENERATE_PARAM=ON" is added on cmake command line)
# See tools/help/custom-config.cmake for details.
#
################################################################################
## subdirectories
set(MOD_DIR_LST 
    "sources/bsp" 
    "sources/board" 
    "sources/device/FlashStorage" 
    "sources/device/Adf7030" 
    "sources/app" 
    )

if(HAS_TRACE_FACILITY)
    set(MOD_DIR_LST ${MOD_DIR_LST} "sources/trace_facility") 
endif(HAS_TRACE_FACILITY)

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

include(sources/app_img.cmake)
