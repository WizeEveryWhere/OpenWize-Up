
string(TIMESTAMP TODAY "%Y-%m-%d %H:%M:%S")

################################################################################
function(display_option)
    message ("      -> USE_PHY_TRIG                    : ${USE_PHY_TRIG}")
    message ("      -> USE_PHY_LAYER_TRACE             : ${USE_PHY_LAYER_TRACE}")
    message ("      -> HAS_HIRES_TIME_MEAS             : ${HAS_HIRES_TIME_MEAS}")
    
    message ("      -> HAS_WIZE_CORE_EXTEND_PARAMETER  : ${HAS_WIZE_CORE_EXTEND_PARAMETER}")
    message ("      -> HAS_LOW_POWER_PARAMETER         : ${HAS_LOW_POWER_PARAMETER}")
    message ("      -> HAS_TEST_MODE_PARAMETER         : ${HAS_TEST_MODE_PARAMETER}")
    message ("      -> HAS_EXTEND_PARAMETER            : ${HAS_EXTEND_PARAMETER}")
    
    message ("      -> BUILD_STANDALAONE_APP           : ${BUILD_STANDALAONE_APP}")
    message ("      -> BUILD_NVM_BINARY                : ${BUILD_NVM_BINARY}")
    
    message ("      -> HAS_ATKEY_CMD                   : ${HAS_ATKEY_CMD}")
    message ("      -> HAS_ATUID_CMD                   : ${HAS_ATUID_CMD}")
    message ("      -> HAS_ATCCLK_CMD                  : ${HAS_ATCCLK_CMD}")
    message ("      -> HAS_ATSTAT_CMD                  : ${HAS_ATSTAT_CMD}")
    message ("      -> HAS_ATZn_CMD                    : ${HAS_ATZn_CMD}")
    
    message ("      -> HW_NAME         : ${HW_NAME}")
    message ("      -> HW_VENDOR       : ${HW_VENDOR}")
    message ("      -> HW_MODEL        : ${HW_MODEL}")
    message ("      -> HW_VER_MAJ      : ${HW_VER_MAJ}")
    message ("      -> HW_VER_MIN      : ${HW_VER_MIN}")
    message ("      -> HW_VER_REV      : ${HW_VER_REV}")
    message ("      -> HW_DATE         : ${HW_DATE}")
endfunction(display_option)

################################################################################
# OpenWizeUp options

set(DEFAULT_CFG_FILE_DIR "sources/app/cfg")
set(PARAM_XML_FILE_LIST "")

# Add LAN Parameters xml files . 
set(PARAM_XML_FILE_LIST "${PARAM_XML_FILE_LIST} ${DEFAULT_CFG_FILE_DIR}/LANParams.xml")

#-------------------------------------------------------------------------------
option(USE_PHY_TRIG                      "Use the PHY trigger pin as TX/RX command" OFF)
option(USE_PHY_LAYER_TRACE               "Enable the PHY layer trace messages." ON)
option(HAS_HIRES_TIME_MEAS               "Define if High-Resolution timer is present (used to get the clock on PONG message)." ON)
option(HAS_WIZE_CORE_EXTEND_PARAMETER    "Use the low power xml file." ON)
option(HAS_LOW_POWER_PARAMETER           "Use the low power xml file." ON)

option(HAS_TEST_MODE_PARAMETER           "Use the test mode xml file." ON)
option(HAS_EXTEND_PARAMETER              "Use the extended parameter xml file." ON)

option(BUILD_STANDALAONE_APP             "Use this option when the bootstrap is not used." OFF)
option(BUILD_NVM_BINARY                  "Build the non-volatile memory area content and produce a binary and elf files." OFF)

# AT cmd option
option(HAS_ATKEY_CMD                     "ATKENC and ATKMAC are replaced by the ATKEY unique command." ON)
option(HAS_ATUID_CMD                     "AT%UID command is defined." ON)
option(HAS_ATCCLK_CMD                    "AT%CCLK command is defined." ON)
option(HAS_ATSTAT_CMD                    "AT%STAT command is defined." ON)
option(HAS_ATZn_CMD                      "ATZ0 and ATZ1 command are defined." ON)

# HW info
set(HW_NAME    "WIZEUP" CACHE STRING "Define the board name print when ATI command is called.")
set(HW_VENDOR  "ALCIOM" CACHE STRING "Define the board vendor print when ATI command is called.")
set(HW_MODEL   "WZ1000" CACHE STRING "Define the board model print when ATI command is called.")
set(HW_VER_MAJ 0 CACHE STRING "Define the board major version number.")
set(HW_VER_MIN 1 CACHE STRING "Define the board minor version number.")
set(HW_VER_REV 0 CACHE STRING "Define the board revision version number.")
set(HW_DATE ${TODAY} CACHE STRING "Define the board date.")

# TODO :
# add_compile_definitions(COM_SWAP_PINS=1)

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
    set(PARAM_XML_FILE_LIST "${PARAM_XML_FILE_LIST} ${DEFAULT_CFG_FILE_DIR}/WizeCoreExtendParams.xml")
endif(HAS_WIZE_CORE_EXTEND_PARAMETER)
#-------------------------------------------------------------------------------
if(USE_LOGGER_SAMPLE)
    set(PARAM_XML_FILE_LIST "${PARAM_XML_FILE_LIST} ${DEFAULT_CFG_FILE_DIR}/LoggerParams.xml")
endif(USE_LOGGER_SAMPLE)
#-------------------------------------------------------------------------------
if(HAS_LOW_POWER_PARAMETER)
    add_compile_definitions(HAS_LP_PARAMETER=1)
    set(PARAM_XML_FILE_LIST "${PARAM_XML_FILE_LIST} ${DEFAULT_CFG_FILE_DIR}/LowPowerParams.xml")
endif(HAS_LOW_POWER_PARAMETER)
#-------------------------------------------------------------------------------
if(HAS_TEST_MODE_PARAMETER)
	add_compile_definitions(HAS_TEST_CFG_PARAMETER=1)
	set(PARAM_XML_FILE_LIST "${PARAM_XML_FILE_LIST} ${DEFAULT_CFG_FILE_DIR}/TestModeParams.xml") 
endif(HAS_TEST_MODE_PARAMETER)
#-------------------------------------------------------------------------------
if(HAS_EXTEND_PARAMETER)
    add_compile_definitions(HAS_EXTEND_PARAMETER=1)
	set(PARAM_XML_FILE_LIST "${PARAM_XML_FILE_LIST} ${DEFAULT_CFG_FILE_DIR}/ExtendParams.xml")
endif(HAS_EXTEND_PARAMETER)
#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
# Add Restriction xml files . 
# Note : restriction xml file must be the last one 
set(PARAM_XML_FILE_LIST "${PARAM_XML_FILE_LIST} ${DEFAULT_CFG_FILE_DIR}/DefaultRestr.xml")

#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
if(BUILD_STANDALAONE_APP)
    add_compile_definitions(BUILD_STANDALAONE_APP=1)
else(BUILD_STANDALAONE_APP)
    set(HAL_CRC_MODULE_ENABLE ON)
    add_compile_definitions(HAS_CRC_COMPUTE=1)
endif(BUILD_STANDALAONE_APP)

#-------------------------------------------------------------------------------
if(BUILD_NVM_BINARY)
    add_compile_definitions(BUILD_NVM_BINARY=1)
endif(BUILD_NVM_BINARY)

#-------------------------------------------------------------------------------
# AT cmd option
if(HAS_ATKEY_CMD)
    add_compile_definitions(HAS_ATKEY_CMD=1)
endif(HAS_ATKEY_CMD)

if(HAS_ATUID_CMD)
    add_compile_definitions(HAS_ATUID_CMD=1)
endif(HAS_ATUID_CMD)

if(HAS_ATSTAT_CMD)
    add_compile_definitions(HAS_ATSTAT_CMD=1)
endif(HAS_ATSTAT_CMD)

if(HAS_ATCCLK_CMD)
    add_compile_definitions(HAS_ATCCLK_CMD=1)
endif(HAS_ATCCLK_CMD)

if(HAS_ATZn_CMD)
    add_compile_definitions(HAS_ATZn_CMD=1)
endif(HAS_ATZn_CMD)

#-------------------------------------------------------------------------------
display_option()
