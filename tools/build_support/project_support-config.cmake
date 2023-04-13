################################################################################
# if not given in command line, set the install prefix to default
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${TOP_INSTALL_DIR} CACHE INTERNAL "")
    message(STATUS "Set CMAKE_INSTALL_PREFIX to default : ${CMAKE_INSTALL_PREFIX}")
endif()

# Set "final" install dir
set_property(GLOBAL PROPERTY INSTALL_DIR "${CMAKE_INSTALL_PREFIX}" )

################################################################################
# Set options
option(DOC_COMPILE "Enable the Documentation compilation" OFF)
option(BUILD_OPENWIZE "" ON)
option(BUILD_TEST "" OFF)

option(USE_FREERTOS "" OFF)


# Get build_support
find_package(build_support REQUIRED)

################################################################################
# if not given in command line
if(NOT BUILD_CFG_DIR)
    # if is given as ENV 
    if($ENV{BUILD_CFG_DIR})
        set(BUILD_CFG_DIR $ENV{BUILD_CFG_DIR} )
    else()
        # set Build Config directory
        set(BUILD_CFG_DIR "${CMAKE_SOURCE_DIR}/sources" )
    endif()
endif()

# Add WizeUp as default
if(NOT DEFINED BUILD_CFG)
    set(BUILD_CFG "WizeUp")
endif()

get_cfg()

################################################################################
# Add others

if(USE_FREERTOS)
    set(BUILD_FREERTOS TRUE)
endif()

if(BUILD_TEST)
    set(BUILD_UNITY TRUE)
endif()

if(BUILD_OPENWIZE)
    set(BUILD_TINYCRYPT TRUE)
endif()

# testing/Unity (must be the first one)
if(BUILD_UNITY)
    message(STATUS "Add Unity Build ")
    add_subdirectory(third-party/testing/Unity)
endif()

# firmware/STM32
if(BUILD_STM)
    message(STATUS "Add STM32 HAL Build ")
    add_subdirectory(third-party/firmware/STM32)
endif()

# rtos/FreeRTOS
if(BUILD_FREERTOS)
    message(STATUS "Add FreeRTOS Build ")
    add_subdirectory(third-party/rtos/FreeRTOS)
endif()

# libraries/Tinycrypt
if(BUILD_TINYCRYPT)
    add_subdirectory(third-party/libraries/Tinycrypt)
endif()

# libraries/OpenWize
if(BUILD_OPENWIZE)
    message(STATUS "Add OpenWize Build ")
    add_subdirectory(third-party/libraries/OpenWize)
endif(BUILD_OPENWIZE)

#################################################################################
# distclean target
set(cmake_generated ${CMAKE_BINARY_DIR}/CMakeCache.txt
                    ${CMAKE_BINARY_DIR}/cmake_install.cmake
                    ${CMAKE_BINARY_DIR}/Makefile
                    ${CMAKE_BINARY_DIR}/CMakeFiles
                    ${CMAKE_BINARY_DIR}/*
)
if(NOT TARGET distclean)
    add_custom_target(distclean COMMAND rm -rf ${cmake_generated} )
    set_property(TARGET distclean PROPERTY FOLDER "CMakePredefinedTargets")
endif()

#################################################################################
# ultraclean target => which is a rm -rf aBuild
if(NOT TARGET ultraclean)
    add_custom_target(ultraclean COMMAND rm -rf "${TOP_BUILD_DIR}/*" "${TOP_INSTALL_DIR}/*" )
    set_property(TARGET ultraclean PROPERTY FOLDER "CMakePredefinedTargets")
endif()

################################################################################
# Add doc rule
if(DOC_COMPILE)
    list(APPEND DOC_SOURCE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/docs")
    string(REPLACE ";" " " DOXYFILE_SOURCE_DIRS "${DOC_SOURCE_DIRS}")
    # Add build support for doxygen
    find_package(doxygen_support)
endif(DOC_COMPILE)