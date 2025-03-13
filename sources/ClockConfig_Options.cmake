#-------------------------------------------------------------------------------
# Main clock default settings

# If not defined here, FW will we use the hard-coded OSC_SOURCE=OSC_MSI
# Possible values are : OSC_MSI, OSC_HSE
add_option(NAME OSC_SOURCE      C_DEFINE DISPLAY)

if(OSC_TRIM)
    # If not defined here, FW will we use the hard-coded OSC_TRIM=1
    # Note : 
    # if OSC_SOURCE == OSC_MSI and LSE succeed :
    # - auto calibrate with MSI_PLL
    # if OSC_SOURCE == OSC_MSI and LSE failed (expected LSI succeed) : 
    # - periodically auto-trimming the oscillator by SW
    # if OSC_SOURCE == OSC_HSE,
    # - don't care, we are not able to trim HSE clock
    if(${OSC_TRIM} EQUAL OFF)
        add_compile_definitions(OSC_TRIM=0)
    endif()
endif()

# Test Purpose only :
# If not defined here, FW will we use the hard-coded OSC_BYPASS=0
# If OSC_SOURCE == OSC_HSE and OSC_BYPASS == 1, then use HSE in bypass mode
add_option(NAME OSC_BYPASS      C_DEFINE DISPLAY_ON)

# Test Purpose only :
# If not defined here, FW will we use the hard-coded OSC_MSI_TRIM=0x0000
# The MSI oscillator trimming value. Used as default and when OSC_TRIM = 0.
add_option(NAME OSC_MSI_TRIM      C_DEFINE)

# Enable / disable the Clock Security System for HSE and LSE
# If not defined here, FW will we use the hard-coded OSC_CSS=0
# - 0b00 : CSS (Clock Security System) disable
# - 0b01 : HSE CSS enable
# - 0b10 : LSE CSS enable
# - 0b11 : HSE and LSE CSS enable
add_option(NAME OSC_CSS       C_DEFINE DISPLAY_ON)

#-------------------------------------------------------------------------------

# Low Speed Clock (LSC) default settings

# If not defined here, FW will we use the hard-coded LSC_DRIVE=LSE_DRIVE_LOW
# Possible values are : 
# LSE_DRIVE_LOW
# LSE_DRIVE_MEDIUMLOW
# LSE_DRIVE_MEDIUMHIGH
# LSE_DRIVE_HIGH
add_option(NAME LSC_DRIVE      C_DEFINE DISPLAY)

# If not defined here, FW will we use the hard-coded LSC_TOGGLE=1
add_option(NAME LSC_TOGGLE     DISPLAY_OFF)

if(DEFINED LSC_TOGGLE)
    # If not defined here, FW will we use the hard-coded LSC_TOGGLE=1
    if( NOT LSC_TOGGLE)
        add_compile_definitions(LSC_TOGGLE=0)
    endif()
endif()


#-------------------------------------------------------------------------------
