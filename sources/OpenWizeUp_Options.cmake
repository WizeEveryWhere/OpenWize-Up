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
endfunction(display_option)

################################################################################
# OpenWizeUp options

set(DEFAULT_CFG_FILE_DIR "sources/app/cfg")
set(PARAM_XML_FILE_LIST "")

# Add LAN Parameters xml files . 
set(PARAM_XML_FILE_LIST "${PARAM_XML_FILE_LIST} ${DEFAULT_CFG_FILE_DIR}/LANParams.xml")

#-------------------------------------------------------------------------------
option(USE_PHY_TRIG                      "TODO : description." OFF)
option(USE_PHY_LAYER_TRACE               "TODO : description." ON)
option(HAS_HIRES_TIME_MEAS               "TODO : description." ON)
option(HAS_WIZE_CORE_EXTEND_PARAMETER    "TODO : description." ON)
option(HAS_LOW_POWER_PARAMETER           "TODO : description." ON)

option(HAS_TEST_MODE_PARAMETER           "TODO : description." ON)
option(HAS_EXTEND_PARAMETER              "TODO : description." ON)

option(BUILD_STANDALAONE_APP             "TODO : description." OFF)
option(BUILD_NVM_BINARY                  "TODO : description." OFF)

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
endif(BUILD_STANDALAONE_APP)

#-------------------------------------------------------------------------------
if(BUILD_NVM_BINARY)
    add_compile_definitions(BUILD_NVM_BINARY=1)
endif(BUILD_NVM_BINARY)


#-------------------------------------------------------------------------------
display_option()