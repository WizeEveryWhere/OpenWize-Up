
*************
API Reference
*************

.. ****************************************************************************

Code organization
=================

Repository structure
--------------------

::

   OpenWize-Up
   ├── docs  
   ├── sources : contains source code specific to OpenWize'Up application and Wize'Up board
   │   ├── app    : OpenWize'Up application
   │   ├── board  : Wize'Up board specific
   │   ├── bsp    : Wize'Up board support package
   │   └── device : Mainly the ADF7030-1 device driver 
   ├── third-party : contains all "external" source code, including OpenWize itself
   │   ├── .OpenWize : OpenWize repository as a submodule (hidden)
   │   ├── firmware  : symbolic link on  OpenWize STM32 HAL submodule 
   │   ├── libraries : symbolic link on  OpenWize Tinycrypt submodule 
   │   ├── rtos      : symbolic link on  OpenWize FreRTOS submodule 
   │   └── testing   : symbolic link on  OpenWize CMock submodule 
   └── tools
       ├── build_support : cmake files to help building OpenWize'Up
       └── scripts       : various bash script


Source directory
----------------

::
 
   sources
   ├── app : Contain the application code
   │   ├── ATCI    : AT command interpreter
   │   ├── extra   : Usefull function to drive the Phy device
   │   ├── cfg     : contains the defaults parameters configuration xml files 
   │   ├── gen     : contains the defaults parameters tables as .c and .h files
   │   ├── include : application include directory
   │   ├── src     : application source directory
   │   ├── sys     : initialize the system modules (RTOS, Logger, Stack...)
   │   └── CMakeLists.txt
   ├── board : Contains the minimum to initialize the board low level (peripherals, clocks)
   ├── bsp : Restricted and simple Board Support Package
   ├── device
   │   ├── Adf7030      : The Analog Device ADF7030-1 driver    
   │   └── FlashStorage : Driver to store/restore from the MCU flash memory 
   ├── FreeRTOSConfig.cmake : FreeRTOS configuration file for this application
   ├── STM32HALConfig.cmake : STM32 HAL configuration file for this board/bsp
   └── WizeUp.cmake         : main cmake file to build and link everything together

.. ****************************************************************************

.. include::  openwize-up.rst

.. toctree::
   :hidden:
