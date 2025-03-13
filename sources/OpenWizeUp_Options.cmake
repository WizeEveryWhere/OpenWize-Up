################################################################################
find_package(gen_param REQUIRED)

set(DBG_OPTION_LIST OFF)

################################################################################
function(add_compile_def_from_options _list_of_options_)
    foreach(_option_ ${_list_of_options_})
        if(NOT DEFINED ${_option_})
            message("Option ${_option_} is not defined.")
        else()
            if(${_option_}) # option is ON or not OFF
                if(${_option_} STREQUAL ON)
                    add_compile_definitions(${_option_}=1)
                    if(DBG_OPTION_LIST)
                        message("Compile definition ${_option_}=1 (${${_option_}})")
                    endif()
                else()
                    add_compile_definitions(${_option_}=${${_option_}})
                    if(DBG_OPTION_LIST)
                        message("Compile definition ${_option_}=${${_option_}}")
                    endif()
                endif()
            else()  # option is OFF
                #message("Option ${_option_}=${${_option_}} (disable)")
            endif()
        endif()
    endforeach(_option_)
endfunction(add_compile_def_from_options)

################################################################################
function(display_var _var_)
    set(_m_len_ 34)
    string(LENGTH ${_var_} _len_)
    math(EXPR _len_ "${_m_len_} - ${_len_}")
    string(REPEAT " " ${_len_} out_space)
    string(CONCAT out_str "      -> ${_var_}" ${out_space} ": ${${_var_}}" )
    message ("${out_str}")
endfunction(display_var)

#---

function(display_if_on _var_)
    if(${_var_})
        display_var(${_var_})
    endif(${_var_})
endfunction(display_if_on)

function(display_if_off _var_)
    if(NOT ${_var_})
        display_var(${_var_})
    endif(NOT ${_var_})
endfunction(display_if_off)

#---

function(display_list _list_)
    foreach(_option_ ${_list_})
        if(DEFINED ${_option_})
            display_var(${_option_})
        endif()
    endforeach(_option_)
endfunction(display_list)

function(display_list_if_on _list_)
    foreach(_option_ ${_list_})
        if(DEFINED ${_option_})
            display_if_on(${_option_})
        endif()
    endforeach(_option_)
endfunction(display_list_if_on)

function(display_list_if_off _list_)
    foreach(_option_ ${_list_})
        if(DEFINED ${_option_})
            display_if_off(${_option_})
        endif()
    endforeach(_option_)
endfunction(display_list_if_off)

################################################################################

function(display_option)
    display_list( "${_HW_LIST_INFO_}" )
    display_list("${_DISPLAY_LIST_}")
    display_list_if_on("${_DISPLAY_LIST_ON_}")
    display_list_if_off("${_DISPLAY_LIST_OFF_}")
endfunction(display_option)

function(add_compile_def)
    if(_C_DEFINE_LIST_)
        add_compile_def_from_options( "${_C_DEFINE_LIST_}" )
    endif()
endfunction(add_compile_def)
################################################################################

function(add_option)
    set(options C_DEFINE DISPLAY DISPLAY_ON DISPLAY_OFF)
    set(oneValueArgs NAME)
    #set(oneValueArgs NAME DESCRIPTION DEFAULT)
    set(multiValueArgs "")
   
    cmake_parse_arguments(ADD_OPTION "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
    
    if(DEFINED ${ADD_OPTION_NAME})
        if(ADD_OPTION_C_DEFINE)
            set(_C_DEFINE_LIST_ ${_C_DEFINE_LIST_} ${ADD_OPTION_NAME} PARENT_SCOPE)
        endif()
        
        if(ADD_OPTION_DISPLAY)
            set(_DISPLAY_LIST_ ${_DISPLAY_LIST_} ${ADD_OPTION_NAME} PARENT_SCOPE)
        elseif(ADD_OPTION_DISPLAY_ON)
            set(_DISPLAY_LIST_ON_ ${_DISPLAY_LIST_ON_} ${ADD_OPTION_NAME} PARENT_SCOPE)
        elseif(ADD_OPTION_DISPLAY_OFF)
            set(_DISPLAY_LIST_OFF_ ${_DISPLAY_LIST_OFF_} ${ADD_OPTION_NAME} PARENT_SCOPE)
        endif()
    else()
        #[[
        set(_description_ "")
        message("Option ${ADD_OPTION_NAME}: ${ADD_OPTION_DEFAULT}; ${ADD_OPTION_DESCRIPTION}")
        if(ADD_OPTION_DEFAULT)
            if(ADD_OPTION_DESCRIPTION)
                set(_description_ ${ADD_OPTION_DESCRIPTION})
            endif()
            option(${ADD_OPTION_NAME} ${_description_} ${ADD_OPTION_DEFAULT})
        endif()
        ]]
        #message(WARNING "Option ${ADD_OPTION_NAME} in not defined")
    endif()
endfunction(add_option)



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
# HW info
set(HW_NAME    "WIZEUP" CACHE STRING "Define the board name print when ATI command is called.")
set(HW_VENDOR  "ALCIOM" CACHE STRING "Define the board vendor print when ATI command is called.")
set(HW_MODEL   "WZ1000" CACHE STRING "Define the board model print when ATI command is called.")
set(HW_VER_MAJ 0 CACHE STRING "Define the board major version number.")
set(HW_VER_MIN 1 CACHE STRING "Define the board minor version number.")
set(HW_VER_REV 0 CACHE STRING "Define the board revision version number.")
set(HW_DATE "2021-05-19 19:16:51" CACHE STRING "Define the board date.")
set(_HW_LIST_INFO_ "HW_NAME;HW_VENDOR;HW_MODEL;HW_VER_MAJ;HW_VER_MIN;HW_VER_REV;HW_DATE")

#-------------------------------------------------------------------------------
# Option
option(USE_LPUART_COM                    "Use the LPUART as COM port (default UART4)" OFF)

option(USE_PHY_TRIG                      "Use the PHY trigger pin as TX/RX command" OFF)
option(USE_PHY_LAYER_TRACE               "Enable the PHY layer trace messages." OFF)
option(USE_PHY_CCA_CFG_VALUE             "Allow CCA cfg register value to be changed." OFF)
option(USE_PHY_CCA_DELAY_VALUE           "Allow to change the delay before the first measured value in the CCA." OFF)
option(USE_PHY_POWER_RAMP                "Enable the PHY PA ramping." OFF)
option(USE_PHY_BYP_DIS_INT               "" ON)

option(HAS_HIRES_TIME_MEAS               "Define if High-Resolution timer is present (used to get the clock on PONG message)." ON)

option(HAS_WIZE_CORE_EXTEND_PARAMETER    "Use the low power xml file." ON)
option(HAS_LOW_POWER_PARAMETER           "Use the low power xml file." ON)
option(HAS_TEST_MODE_PARAMETER           "Use the test mode xml file." ON)
option(HAS_EXTEND_PARAMETER              "Use the extended parameter xml file." ON)
option(HAS_IO_CFG_PARAMETER              "Use the io configuration xml file." OFF)

# 
option(HAS_NO_BANNER                       "Don't display the Wize banner." ON)
option(USE_RTOS_TRACE                      "Enable the FreeRTOS task and queue naming trace" ON)
option(HAS_BSP_PWRLINE                     "..." ON)
option(LOGGER_HAS_COLOR                    "Enable the logger coloring" ON)
option(LOGGER_USE_FWRITE                   "Enable the use of libc fwrite (otherwise use fputs)." ON)
option(LOGGER_USE_ROTATE                   "Enable the use of rotate logs." OFF)
option(DUMP_CORE_HAS_TRACE                 "Trace / display the core dump on crash" ON)
option(DUMP_CORE_HAS_FAULT_STATUS_REGISTER "Display fault status register with core dump." ON)

#
option(BUILD_STANDALONE_APP              "Use this option when the bootstrap is not used." OFF)
option(BUILD_NVM_BINARY                  "Build the non-volatile memory area content and produce a binary and elf files." OFF)

# Just for test purpose
option(NOT_BOOTABLE "Use this option to build a not bootable image (test purpose only)" OFF)

set(L6VERS                 L6VER_WIZE_REV_1_2)
set(AT_CMD_MAX_NB_PARAM    7)
set(ADF7030_1_SPI_FAST_SIZE_THRESHOLD   288)

option(USE_SPI                     "..." ON)
option(USE_I2C                     "..." OFF)

# Add tracing facility
option(HAS_TRACE_FACILITY "Enable trace facility" OFF)


option(OPTIMIZE_WIZECORE "Optimize WizeCore lib for size" OFF)
option(OPTIMIZE_SAMPLE "Optimize Sample lib for size" OFF)
option(OPTIMIZE_TINYCRYPT "Optimize Tinycrypt lib for size" OFF)
option(OPTIMIZE_FREERTOS "Optimize FreeRTOS lib for size" OFF)
option(OPTIMIZE_STM "Optimize STM32 lib for size" OFF)


# ...just for compatibility
if(USE_LPUART_COM)
    set(LOGGER_DEV_MAP  DEV_ID_1)
    set(CONSOLE_DEV_MAP DEV_ID_1)
endif(USE_LPUART_COM)

#message("...LOGGER_DEV_MAP = ${LOGGER_DEV_MAP}")

#-------------------------------------------------------------------------------
set(_C_DEFINE_LIST_ "")
set(_DISPLAY_LIST_ "")
set(_DISPLAY_LIST_ON_ "")
set(_DISPLAY_LIST_OFF_ "")

add_option(NAME USE_LPUART_COM                   DISPLAY_ON)

add_option(NAME USE_PHY_TRIG            C_DEFINE DISPLAY_ON)
add_option(NAME USE_PHY_LAYER_TRACE     C_DEFINE DISPLAY_ON)
add_option(NAME USE_PHY_CCA_CFG_VALUE   C_DEFINE DISPLAY_ON)
add_option(NAME USE_PHY_CCA_DELAY_VALUE C_DEFINE DISPLAY_ON)
add_option(NAME USE_PHY_POWER_RAMP      C_DEFINE DISPLAY_ON)
add_option(NAME USE_PHY_BYP_DIS_INT     C_DEFINE)

add_option(NAME HAS_HIRES_TIME_MEAS     C_DEFINE DISPLAY_ON)
add_option(NAME HAS_WIZE_CORE_EXTEND_PARAMETER C_DEFINE DISPLAY_ON)
add_option(NAME HAS_LOW_POWER_PARAMETER C_DEFINE DISPLAY_ON)
add_option(NAME HAS_TEST_MODE_PARAMETER C_DEFINE DISPLAY_ON)
add_option(NAME HAS_EXTEND_PARAMETER    C_DEFINE DISPLAY_ON)
add_option(NAME HAS_IO_CFG_PARAMETER    C_DEFINE DISPLAY_ON)

add_option(NAME HAS_NO_BANNER           C_DEFINE)
add_option(NAME USE_RTOS_TRACE          C_DEFINE)
add_option(NAME HAS_TRACE_FACILITY      C_DEFINE DISPLAY_ON)

add_option(NAME HAS_BSP_PWRLINE         C_DEFINE)
add_option(NAME LOGGER_HAS_COLOR        C_DEFINE)
add_option(NAME LOGGER_USE_FWRITE       C_DEFINE)
add_option(NAME LOGGER_USE_ROTATE       C_DEFINE)
add_option(NAME DUMP_CORE_HAS_TRACE     C_DEFINE)
add_option(NAME DUMP_CORE_HAS_FAULT_STATUS_REGISTER C_DEFINE)

add_option(NAME BUILD_STANDALONE_APP    C_DEFINE DISPLAY_ON)
add_option(NAME BUILD_NVM_BINARY        C_DEFINE DISPLAY_ON)

add_option(NAME NOT_BOOTABLE            C_DEFINE DISPLAY_ON)
add_option(NAME L6VERS                  C_DEFINE)
add_option(NAME AT_CMD_MAX_NB_PARAM     C_DEFINE)
add_option(NAME ADF7030_1_SPI_FAST_SIZE_THRESHOLD     C_DEFINE)

add_option(NAME USE_SPI                 C_DEFINE)
add_option(NAME USE_I2C                 C_DEFINE)

#[[
add_option(NAME OPTIMIZE_WIZECORE       DISPLAY)
add_option(NAME OPTIMIZE_SAMPLE         DISPLAY)
add_option(NAME OPTIMIZE_TINYCRYPT      DISPLAY)
add_option(NAME OPTIMIZE_FREERTOS       DISPLAY)
add_option(NAME OPTIMIZE_STM            DISPLAY)
]]

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
# Some Default HW settings

add_option(NAME USE_AUTOCLK      C_DEFINE DISPLAY_ON)
if(USE_AUTOCLK)
    if(HAS_IO_CFG_PARAMETER)
        # The LOGGER_DEV_MAP and CONSOLE_DEV_MAP should be defined in IoParams.xml.  
        add_xml_cfg(DESTINATION PARAM_XML_FILE_LIST SOURCE "IoParams.xml")
    else()
        # -------------------------------------
        # Clock default settings
        include(sources/ClockConfig_Options.cmake)
        
        # -------------------------------------
        # Serial port default settings
        include(sources/SerialConfig_Options.cmake)
    
        # Serial possible mapping : 
        # DEV_ID_0 (aka UART4   on PA0 :TX and PA1 :RX)
        # DEV_ID_1 (aka LPUART1 on PB11:TX and PB10:RX)
        # DEV_ID_2 (aka USART1  on PA9 :TX and PA10:RX)
        # DEV_ID_3 (aka USART3  on PB10:TX and PB11:RX)

        # Logger serial mapping
        # If not defined here, FW will we use the hard-coded LOGGER_DEV_MAP=DEV_ID_0    
        add_option(NAME LOGGER_DEV_MAP            C_DEFINE DISPLAY)

        # Console serial mapping
        # If not defined here, FW will we use the hard-coded CONSOLE_DEV_MAP=DEV_ID_0   
        add_option(NAME CONSOLE_DEV_MAP            C_DEFINE DISPLAY)

    endif(HAS_IO_CFG_PARAMETER)
endif(USE_AUTOCLK)
#-------------------------------------------------------------------------------
if(HAS_WIZE_CORE_EXTEND_PARAMETER)
    add_xml_cfg(DESTINATION PARAM_XML_FILE_LIST SOURCE "WizeCoreExtendParams.xml")
endif(HAS_WIZE_CORE_EXTEND_PARAMETER)
#-------------------------------------------------------------------------------
if(USE_LOGGER_SAMPLE)
    add_xml_cfg(DESTINATION PARAM_XML_FILE_LIST SOURCE "LoggerParams.xml")
endif(USE_LOGGER_SAMPLE)
#-------------------------------------------------------------------------------
if(HAS_LOW_POWER_PARAMETER)
    add_xml_cfg(DESTINATION PARAM_XML_FILE_LIST SOURCE "LowPowerParams.xml")
endif(HAS_LOW_POWER_PARAMETER)
#-------------------------------------------------------------------------------
if(HAS_TEST_MODE_PARAMETER)
	add_xml_cfg(DESTINATION PARAM_XML_FILE_LIST SOURCE "TestModeParams.xml") 
endif(HAS_TEST_MODE_PARAMETER)
#-------------------------------------------------------------------------------
if(HAS_EXTEND_PARAMETER)
	add_xml_cfg(DESTINATION PARAM_XML_FILE_LIST SOURCE "ExtendParams.xml")
endif(HAS_EXTEND_PARAMETER)
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Add Restriction xml files . 
# Note : restriction xml file must be the last one 
add_xml_cfg(DESTINATION PARAM_XML_FILE_LIST SOURCE "DefaultRestr.xml")

################################################################################
add_compile_def()
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
