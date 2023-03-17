# OpenWizeUp options

set(DEFAULT_CFG_FILE_DIR "sources/app/cfg")
set(PARAM_XML_FILE_LIST "")

# Add LAN Parameters xml files . 
set(PARAM_XML_FILE_LIST "${PARAM_XML_FILE_LIST} ${DEFAULT_CFG_FILE_DIR}/LANParams.xml")

#-------------------------------------------------------------------------------
option(USE_PHY_TRIG            "TODO : description." OFF)
option(USE_PHY_LAYER_TRACE     "TODO : description." ON)
option(HAS_TEST_MODE_PARAMETER "TODO : description." ON)
option(HAS_LOW_POWER_PARAMETER "TODO : description." ON)
option(HAS_EXTEND_PARAMETER    "TODO : description." OFF)

#-------------------------------------------------------------------------------
if(USE_PHY_TRIG)
	add_compile_definitions(USE_PHY_TRIG=1)
endif(USE_PHY_TRIG)
#-------------------------------------------------------------------------------
if(USE_PHY_LAYER_TRACE)
	add_compile_definitions(USE_PHY_LAYER_TRACE=1)
endif(USE_PHY_LAYER_TRACE)
#-------------------------------------------------------------------------------
if(HAS_TEST_MODE_PARAMETER)
	add_compile_definitions(HAS_TEST_CFG_PARAMETER=1)
	set(PARAM_XML_FILE_LIST "${PARAM_XML_FILE_LIST} ${DEFAULT_CFG_FILE_DIR}/TestModeParams.xml") 
endif(HAS_TEST_MODE_PARAMETER)
#-------------------------------------------------------------------------------
if(HAS_LOW_POWER_PARAMETER)
	add_compile_definitions(HAS_LP_PARAMETER=1)
	set(PARAM_XML_FILE_LIST "${PARAM_XML_FILE_LIST} ${DEFAULT_CFG_FILE_DIR}/LowPowerParams.xml")
endif(HAS_LOW_POWER_PARAMETER)
#-------------------------------------------------------------------------------
if(HAS_EXTEND_PARAMETER)
	set(PARAM_XML_FILE_LIST "${PARAM_XML_FILE_LIST} ${DEFAULT_CFG_FILE_DIR}/ExtendParams.xml")
endif(HAS_EXTEND_PARAMETER)
#-------------------------------------------------------------------------------
if(USE_LOGGER_SAMPLE)
	set(PARAM_XML_FILE_LIST "${PARAM_XML_FILE_LIST} ${DEFAULT_CFG_FILE_DIR}/LoggerParams.xml")
endif(USE_LOGGER_SAMPLE)

#-------------------------------------------------------------------------------
# Add Restriction xml files . 
# Note : restriction xml file must be the last one 
set(PARAM_XML_FILE_LIST "${PARAM_XML_FILE_LIST} ${DEFAULT_CFG_FILE_DIR}/DefaultRestr.xml")

#-------------------------------------------------------------------------------
