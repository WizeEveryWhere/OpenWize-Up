cmake_minimum_required( VERSION 3.12 ) 
################################################################################

set(BOARD_NAME "Alciom Wize'Up Board")
set(APP_NAME "Bootstrap")

add_compile_options(-Wall -ffunction-sections -fdata-sections -fstack-usage)
add_compile_options(-Os)

################################################################################
# Set Coss compile
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CROSSCOMPILING TRUE )

# Disable OpenWize build
set(BUILD_OPENWIZE OFF)

# Set MCU
set(VENDOR_MCU "STM32L451xx")

set(HAL_CONF_FILE "${CMAKE_CURRENT_SOURCE_DIR}/sources/bootstrap/STM32HALConfig.cmake")

################################################################################
find_package(cross_stm32 REQUIRED)
set_cross_stm32()

find_package(toolchain REQUIRED)

################################################################################
## subdirectories
set(MOD_DIR_LST 
    "sources/bootstrap" 
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
