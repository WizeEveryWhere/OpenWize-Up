################################################################################
# Customization :
# On command line add custom directory with -DCUSTOM_DIR=my_custom
# 1. From the "my_custom/cfg" directory all known xml file name will used in 
#    place of the default ones (located in sources/app/cfg). 
# 2. Add "BusinessParams.xml" file if exist in "my_custom/cfg"
# 3. Include the file "custom-config.cmake" (as cmake package) if it exist in 
#    "my_custom" directory.
#
# Note :
# - You can redefine the "BusinessParams.xml" file in "custom-config.cmake" by
#   setting CUSTOM_BUSINESS_XML (e.g. set(CUSTOM_BUSINESS_XML "MyOnwPar.xml"). 
# - You can redefine most of variable in "custom-config.cmake".
#
################################################################################

#-------------------------------------------------------------------------------
# Redefine HW info (Given from ATI command)
#set(HW_NAME "NAME")
#set(HW_VENDOR "VENDOR")
#set(HW_MODEL "DODEL")
#set(HW_VER_MAJ 0)
#set(HW_VER_MIN 1)
#set(HW_VER_REV 0)
#set(HW_DATE "2012-12-12 12:12:12")

#-------------------------------------------------------------------------------
# Redefine some configuration variable
#set(USE_LPUART_COM ON)
#set(BUILD_STANDALONE_APP ON)
#set(BUILD_NVM_BINARY ON)

#-------------------------------------------------------------------------------
# Redefine the business xml file (if any). Default is "BusinessParams.xml".
#set(CUSTOM_BUSINESS_XML "MyBusinessParams.xml")

#-------------------------------------------------------------------------------
# Redefine the executable (.elf, .bin, ...) file. Default is "App_WizeUp".
#set(CUSTOM_BIN_NAME "custom_app")

#-------------------------------------------------------------------------------
# Example :
# From OpenWize-Up directory
# 
# $> mkdir ../my_custom
# $> mkdir ../my_custom/cfg
# $> cp ./sources/cfg/LoggerParams.xml ../my_custom/cfg/.
# $> cp ./tools/help/cutom-config.cmake ../my_custom/.
#
# $> mkdir _build_custom
# $> cd _build_custom
#
# Parameters generation is required
# $> cmake -DCUSTOM_DIR=../my_custom -DGENERATE_PARAM=ON -DCMAKE_BUILD_TYPE=Debug ../.
#
# Parameters generation is not required
# $> cmake -DCUSTOM_DIR=../my_custom -DCMAKE_BUILD_TYPE=Debug ../.
#
# Redefine installation directory
# $> cmake -DCMAKE_INSTALL_PREFIX=../_install_custom -DCMAKE_BUILD_TYPE=Debug  ../.
#
#
# $> make -j
# $> make install
#

