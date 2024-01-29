################################################################################
find_package(gen_param REQUIRED)

################################################################################
function(display_option)
    message ("      -> USE_LPUART_COM                  : ${USE_LPUART_COM}")

    message ("      -> USE_PHY_TRIG                    : ${USE_PHY_TRIG}")
    message ("      -> USE_PHY_LAYER_TRACE             : ${USE_PHY_LAYER_TRACE}")
    message ("      -> HAS_HIRES_TIME_MEAS             : ${HAS_HIRES_TIME_MEAS}")
    
    message ("      -> HAS_WIZE_CORE_EXTEND_PARAMETER  : ${HAS_WIZE_CORE_EXTEND_PARAMETER}")
    message ("      -> HAS_LOW_POWER_PARAMETER         : ${HAS_LOW_POWER_PARAMETER}")
    message ("      -> HAS_TEST_MODE_PARAMETER         : ${HAS_TEST_MODE_PARAMETER}")
    message ("      -> HAS_EXTEND_PARAMETER            : ${HAS_EXTEND_PARAMETER}")
    
    message ("      -> BUILD_STANDALONE_APP            : ${BUILD_STANDALONE_APP}")
    message ("      -> BUILD_NVM_BINARY                : ${BUILD_NVM_BINARY}")
   
    message ("      -> HW_NAME         : ${HW_NAME}")
    message ("      -> HW_VENDOR       : ${HW_VENDOR}")
    message ("      -> HW_MODEL        : ${HW_MODEL}")
    message ("      -> HW_VER_MAJ      : ${HW_VER_MAJ}")
    message ("      -> HW_VER_MIN      : ${HW_VER_MIN}")
    message ("      -> HW_VER_REV      : ${HW_VER_REV}")
    message ("      -> HW_DATE         : ${HW_DATE}")   
endfunction(display_option)

################################################################################
string(TIMESTAMP TODAY "%Y-%m-%d %H:%M:%S")

# OpenWizeUp options
set(DEFAULT_CFG_GEN_DIR "sources/app")
set(DEFAULT_CFG_FILE_DIR "sources/app")

set(PARAM_XML_FILE_LIST "")

if(NOT CUSTOM_DIR)
    set(CFG_GEN_DIR "${DEFAULT_CFG_GEN_DIR}")
else()
    set(CFG_GEN_DIR "${CUSTOM_DIR}")
endif()

# Add LAN Parameters xml files . 
add_xml_cfg(DESTINATION PARAM_XML_FILE_LIST SOURCE "LANParams.xml")

#-------------------------------------------------------------------------------
# Option
option(USE_LPUART_COM                    "Use the LPUART as COM port (default UART4)" OFF)

option(USE_PHY_TRIG                      "Use the PHY trigger pin as TX/RX command" OFF)
option(USE_PHY_LAYER_TRACE               "Enable the PHY layer trace messages." OFF)
option(HAS_HIRES_TIME_MEAS               "Define if High-Resolution timer is present (used to get the clock on PONG message)." ON)
option(HAS_WIZE_CORE_EXTEND_PARAMETER    "Use the low power xml file." ON)
option(HAS_LOW_POWER_PARAMETER           "Use the low power xml file." ON)

option(HAS_TEST_MODE_PARAMETER           "Use the test mode xml file." ON)
option(HAS_EXTEND_PARAMETER              "Use the extended parameter xml file." ON)

option(BUILD_STANDALONE_APP              "Use this option when the bootstrap is not used." OFF)
option(BUILD_NVM_BINARY                  "Build the non-volatile memory area content and produce a binary and elf files." OFF)

# Just for test purpose
option(NOT_BOOTABLE "Use this option to build a not bootable image (test purpose only)" OFF)

# HW info
set(HW_NAME    "WIZEUP" CACHE STRING "Define the board name print when ATI command is called.")
set(HW_VENDOR  "ALCIOM" CACHE STRING "Define the board vendor print when ATI command is called.")
set(HW_MODEL   "WZ1000" CACHE STRING "Define the board model print when ATI command is called.")
set(HW_VER_MAJ 0 CACHE STRING "Define the board major version number.")
set(HW_VER_MIN 1 CACHE STRING "Define the board minor version number.")
set(HW_VER_REV 0 CACHE STRING "Define the board revision version number.")
set(HW_DATE "2021-05-19 19:16:51" CACHE STRING "Define the board date.")

# TODO :
# add_compile_definitions(COM_SWAP_PINS=1)

#-------------------------------------------------------------------------------
# Add custom.cmake if any
if(CUSTOM_DIR)
    find_package(custom PATHS ${CUSTOM_DIR} NO_DEFAULT_PATH)
    if(NOT CUSTOM_BUSINESS_XML)
        set(CUSTOM_BUSINESS_XML "BusinessParams.xml")
    endif()
    find_file(CUSTOM_BUSINESS_XML_FILE ${CUSTOM_BUSINESS_XML} PATHS ${CUSTOM_DIR} PATH_SUFFIXES cfg NO_DEFAULT_PATH )
    if(CUSTOM_BUSINESS_XML_FILE)
        add_xml_cfg(DESTINATION PARAM_XML_FILE_LIST SOURCE ${CUSTOM_BUSINESS_XML} )
    #else()
    #    message("WARNING : \"${CUSTOM_BUSINESS_XML}\" file not found")
    endif()
endif()

#-------------------------------------------------------------------------------
if(USE_LPUART_COM)
    add_compile_definitions(USE_UART_COM_ID=1)
    add_compile_definitions(USE_UART_LOG_ID=1)
    add_compile_definitions(USE_LPUART1=1)
else(USE_LPUART_COM)
    add_compile_definitions(USE_UART_COM_ID=0)
    add_compile_definitions(USE_UART_LOG_ID=0)
    add_compile_definitions(USE_UART4=1)
endif(USE_LPUART_COM)
#-------------------------------------------------------------------------------
if(USE_PHY_TRIG)
	add_compile_definitions(USE_PHY_TRIG=1)
endif(USE_PHY_TRIG)
#-------------------------------------------------------------------------------
if(USE_PHY_LAYER_TRACE)
	add_compile_definitions(USE_PHY_LAYER_TRACE=1)
endif(USE_PHY_LAYER_TRACE)
#-------------------------------------------------------------------------------
if(HAS_HIRES_TIME_MEAS)
    add_compile_definitions(HAS_HIRES_TIME_MEAS=1)
endif(HAS_HIRES_TIME_MEAS)
#-------------------------------------------------------------------------------
if(HAS_WIZE_CORE_EXTEND_PARAMETER)
    add_compile_definitions(HAS_WIZE_CORE_EXTEND_PARAMETER=1)
    add_xml_cfg(DESTINATION PARAM_XML_FILE_LIST SOURCE "WizeCoreExtendParams.xml")
endif(HAS_WIZE_CORE_EXTEND_PARAMETER)
#-------------------------------------------------------------------------------
if(USE_LOGGER_SAMPLE)
    add_xml_cfg(DESTINATION PARAM_XML_FILE_LIST SOURCE "LoggerParams.xml")
endif(USE_LOGGER_SAMPLE)
#-------------------------------------------------------------------------------
if(HAS_LOW_POWER_PARAMETER)
    add_compile_definitions(HAS_LP_PARAMETER=1)
    add_xml_cfg(DESTINATION PARAM_XML_FILE_LIST SOURCE "LowPowerParams.xml")
endif(HAS_LOW_POWER_PARAMETER)
#-------------------------------------------------------------------------------
if(HAS_TEST_MODE_PARAMETER)
	add_compile_definitions(HAS_TEST_CFG_PARAMETER=1)
	add_xml_cfg(DESTINATION PARAM_XML_FILE_LIST SOURCE "TestModeParams.xml") 
endif(HAS_TEST_MODE_PARAMETER)
#-------------------------------------------------------------------------------
if(HAS_EXTEND_PARAMETER)
    add_compile_definitions(HAS_EXTEND_PARAMETER=1)
	add_xml_cfg(DESTINATION PARAM_XML_FILE_LIST SOURCE "ExtendParams.xml")
endif(HAS_EXTEND_PARAMETER)
#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
# Add Restriction xml files . 
# Note : restriction xml file must be the last one 
add_xml_cfg(DESTINATION PARAM_XML_FILE_LIST SOURCE "DefaultRestr.xml")

#-------------------------------------------------------------------------------
if(BUILD_STANDALONE_APP)
    add_compile_definitions(BUILD_STANDALONE_APP=1)
endif(BUILD_STANDALONE_APP)

if(LOWPOWER_DEBUG)
    add_compile_definitions(LOWPOWER_DEBUG=1)
endif(LOWPOWER_DEBUG)

#-------------------------------------------------------------------------------
if(BUILD_NVM_BINARY)
    add_compile_definitions(BUILD_NVM_BINARY=1)
endif(BUILD_NVM_BINARY)

# Just for test purpose ONLY
if(NOT_BOOTABLE)
    add_compile_definitions(NOT_BOOTABLE=1)
endif(NOT_BOOTABLE)

################################################################################

display_option()

#-------------------------------------------------------------------------------
if(custom_FOUND)
    message("   -----------------------------")
    message("   -> \"custom-config.cmake\" package found in \"${CUSTOM_DIR}\" directory")
endif()

set(opt 0)
if(BUILD_NVM_BINARY)
    set(opt 1)
endif(BUILD_NVM_BINARY)

gen_param(SOURCE ${PARAM_XML_FILE_LIST} DESTINATION ${CFG_GEN_DIR} OPT ${opt})

################################################################################
