################################################################################
# include the build support cmake
find_package(install_support REQUIRED)

set(MODULE_NAME App_WizeUp)

################################################################################
add_subdirectory(ATCI)

# Add executable 
add_executable(${MODULE_NAME})

# Add sources to Build
target_sources(${MODULE_NAME}
    PRIVATE
        src/app_entry.c
        src/storage.c
        sys/port.c
        sys/rtos.c
        sys/sys_init.c
        sys/default_device_config.c 
        gen/parameters_cfg.c
        gen/parameters_default.c
        extra/phy_test.c 
    )
    
# Add include dir    
target_include_directories(
    ${MODULE_NAME} 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/sys
        ${CMAKE_CURRENT_SOURCE_DIR}/gen
        ${CMAKE_CURRENT_SOURCE_DIR}/extra
    )

# Add some link flags
target_link_options(${MODULE_NAME}
    PUBLIC
        --specs=nano.specs 
        -u_printf_float 
        -Wl,-Map=${MODULE_NAME}.map
    )

################################################################################
setup_install(
    TARGET ${MODULE_NAME} 
    DEPENDS 
        Board_WizeUp 
        atci 
        adf7030 
        flashstorage 
        bsp 
        Openwize::Openwize
    )
