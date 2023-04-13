################################################################################
# include the build support cmake
find_package(build_support REQUIRED)

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
        update/update.c
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
        ${CMAKE_CURRENT_SOURCE_DIR}/update
    )

# Add some link flags
target_link_options(${MODULE_NAME}
    PUBLIC
        --specs=nano.specs 
        -u_printf_float 
        -Wl,-Map=${MODULE_NAME}.map
        -Wl,--gc-sections 
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
################################################################################
if(BUILD_NVM_BINARY)

set(MODULE_NAME nvm_area)
add_executable(${MODULE_NAME})

# Add sources to Build
target_sources(
    ${MODULE_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/nvm/nvm_area.c
    )

# Add include dir    
target_include_directories(
    ${MODULE_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/gen
    )

# Add some link flags
set(LD_SCRIPT STM32L451CEUX_def.ld)
target_link_options(${MODULE_NAME}
    PUBLIC
        --specs=nosys.specs
        -Wl,-Map=${MODULE_NAME}.map 
        "-T${CMAKE_CURRENT_SOURCE_DIR}/../board/ld/${LD_SCRIPT}"
        "-T${CMAKE_CURRENT_SOURCE_DIR}/../board/ld/definition.ld"
        "-T${CMAKE_CURRENT_SOURCE_DIR}/nvm/nvm_sections.ld"
    )
    
setup_install(
    TARGET ${MODULE_NAME} 
    DEPENDS 
        adf7030 
        flashstorage 
        Openwize::Openwize
    )

set(NVM_STRIP_ELF "$<TARGET_FILE_DIR:${MODULE_NAME}>/${MODULE_NAME}_strip.elf")
set(NVM_STRIP_BIN "$<TARGET_FILE_DIR:${MODULE_NAME}>/${MODULE_NAME}_strip.bin")
add_custom_command(
    TARGET ${MODULE_NAME} POST_BUILD
    COMMAND "${CMAKE_OBJCOPY}" -j .nvm "$<TARGET_FILE:${MODULE_NAME}>" "${NVM_STRIP_ELF}"
    COMMAND "${CMAKE_OBJCOPY}" -O binary "${NVM_STRIP_ELF}" "${NVM_STRIP_BIN}"
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${MODULE_NAME}_strip.elf DESTINATION bin)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${MODULE_NAME}_strip.bin DESTINATION bin)

endif(BUILD_NVM_BINARY)
################################################################################