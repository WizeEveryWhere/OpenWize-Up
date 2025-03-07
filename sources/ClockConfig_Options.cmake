function(display_clock_setting)  
	message ("      -> Clock setting:")
	if(DEFINED OSC_SOURCE)
	    message ("         -> OSC_SOURCE  : ${OSC_SOURCE}")
	    message ("         -> PLL_M       : ${PLL_M}")
	    message ("         -> PLL_N       : ${PLL_N}")
	    message ("         -> PLL_R       : ${PLL_R}")
    endif()
    if(DEFINED LSC_DRIVE)
    	message ("         -> LSC_DRIVE   : ${LSC_DRIVE}")
    endif()
    if(DEFINED LSC_TOGGLE)
    	message ("         -> LSC_TOGGLE  : ${LSC_TOGGLE}")
    endif()
endfunction(display_clock_setting)

add_compile_definitions(CUSTOM_THE_126=1)
add_compile_definitions(OSC_MSI=0)
add_compile_definitions(OSC_HSE=1)

#-------------------------------------------------------------------------------
# Main clock default settings

# If not defined here, FW will we use the hard-coded OSC_SOURCE=OSC_MSI
# Possible values are : OSC_MSI, OSC_HSE
# If OSC_SOURCE == OSC_HSE, the PLL is required to target optimal frequency.
# - The PLL factors () are required to target the 48Mhz.
# - Depends of your HSE crystal frequency (see the STM32L451 Reference manual)
# - If required, modify the "HSE_VALUE" in "STM32HALConfig.cmake file?
#  
if(DEFINED OSC_SOURCE)
    if(${OSC_SOURCE} STREQUAL "OSC_HSE")
    	if(NOT DEFINED PLL_M )
	    	message(FATAL_ERROR "PLL_M must be set to be set to use the OSC_HSE")
	    endif()
	    if(NOT DEFINED PLL_N)
	    	message(FATAL_ERROR "PLL_N must be set to be set to use the OSC_HSE")
	    endif()
	    if(NOT DEFINED PLL_R)
	    	message(FATAL_ERROR "PLL_R must be set to be set to use the OSC_HSE")
	    endif()
	    add_compile_definitions(OSC_SOURCE=${OSC_SOURCE})
    	add_compile_definitions(PLL_M=${PLL_M})
    	add_compile_definitions(PLL_N=${PLL_N})
    	add_compile_definitions(PLL_R=${PLL_R})
    endif()
endif()

#-------------------------------------------------------------------------------

# Low Speed Clock (LSC) default settings

# If not defined here, FW will we use the hard-coded LSC_DRIVE=RCC_LSEDRIVE_LOW
# Possible values are : 
# RCC_LSEDRIVE_LOW
# RCC_LSEDRIVE_MEDIUMLOW 
# RCC_LSEDRIVE_MEDIUMHIGH
# RCC_LSEDRIVE_HIGH

if(DEFINED LSC_DRIVE)
	add_compile_definitions(LSC_DRIVE=${LSC_DRIVE})
endif()

# If not defined here, FW will we use the hard-coded LSC_TOGGLE=1
if(DEFINED LSC_TOGGLE)
    # If not defined here, FW will we use the hard-coded LSC_TOGGLE=1
    if( NOT LSC_TOGGLE)
        add_compile_definitions(LSC_TOGGLE=0)
    endif()
endif()

#-------------------------------------------------------------------------------
