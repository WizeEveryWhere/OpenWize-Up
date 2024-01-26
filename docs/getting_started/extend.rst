
.. ****************************************************************************

Bootstrap and memory mapping
============================

The OpenWize-Up application can be build in two different mode : with or without "bootstrap".

The bootstrap is a very simple piece of code with the roles to :
   - Swap the application FW between the update and backup version
   - Activate the WatchDog
   - Manage the "boot counter"
   - Activate the memory protection

.. warning:: 
   The bootstrap is very specific to the STM32L451 MCU, but should be portable 
   on most of STM32L4 MCU series.

By default the application assume a bootstrap support. To build the application
without the bootstrap, add the "-DBUILD_STANDALONE_APP=ON" option to the cmake 
command line.
Note that without the bootstrap, the final stage of application update or 
roll-back process is not possible.  

The bootstrap is built separately and assume the following predefined memory mapping (see `memory_mapping.txt`_ for details) : 

.. code-block::

    _____ ______________________________________________________________________
   |     |                    |       |                    |                    |
   |  B  |         A          |   S   |         I0         |         I1         |
   |_____|____________________|_______|____________________|____________________|
   
   - B  : BootStrap partition
   - A  : Active part : is where the application is executed. Application has to be linked for this area.
   - S  : Storage part : used by the application as NVM to store permanent information.
   - I0 : Inactive part 0 : used as "update" or "backup" area.
   - I1 : Inactive part 1 : used as "update" or "backup" area.


To be able to use the bootstrap the processes is the following  :

#. Build the bootstrap

   .. code-block:: bash
   
      cd OpenWize-Up
      mkdir -p _build_bs
      cd _build_bs
      cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_CFG=Bootstrap  ../.
      make -j
      make install

#. Build the application and (optionally) the NVM 

   .. code-block:: bash
   
      cd OpenWize-Up
      mkdir -p _build
      cd _build
      cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_NVM_BINARY=ON ../.
      make -j
      make install

#. Generate the application image for part I0 (img 0) and part I1 (img 1)

Use the script `img_gen.sh`_ in "tools/scripts" directory : 

   .. code-block:: bash
   
      cd OpenWize-Up
      tools/scripts/img_gen.sh -i 0
      tools/scripts/img_gen.sh -i 1

#. Load all firmwares (img 0, img 1, bootstrap and optionally NVM)

   .. code-block:: bash
   
      tool="STM32_Programmer_CLI -c port=SWD mode=UR reset=HWrst index=0";
      
      IMG_0_BIN="_install/bin/img_0.bin";
      IMG_1_BIN="_install/bin/img_1.bin";
      NVM="_install/bin/nvm_area_strip.bin";
      BOOT_BIN="_install/bin/bootstrap.bin";
      
      ${tool} --skipErase --erase all \
         -d ${IMG_0_BIN} 0x0802c000 \
         -d ${IMG_1_BIN} 0x08056000 \
         -d ${NVM} 0x0802b000 \
         -d ${BOOT_BIN} 0x08000000;


At reboot, the bootstrap check if a valid FW exist on I0, I1 and A part. Then, 
depending on each part state's, the bootstrap will execute some actions as 
describe in the following table :  

.. list-table:: 
   :widths: 14 14 14 40 30
   :header-rows: 1
   
   * - A
     - I0
     - I1
     - Bootstrap Actions
     - Comments
   * - Invalid
     - Invalid
     - Invalid
     - "Panic". Infinite reboot loop.
     - This should never happened 
   * - Invalid
     - Invalid
     - Valid
     - Copy/Paste I1 to A, then reboot.
     - Update Part will be I0.
   * - Invalid
     - Valid
     - Invalid
     - Copy/Paste I0 to A, then reboot.
     - Update Part will be I1.
   * - Valid
     - Invalid
     - Invalid
     - Jump to A.
     - Update Part will be I0.
   * - Valid
     - Invalid
     - Valid
     - Jump to A.
     - Update Part will be I0.
   * - Valid
     - Valid
     - Invalid
     - Jump to A.
     - Update Part will be I1.
   * - Valid
     - Valid
     - Valid
     - Jump to A.
     - | If A == I0 : Update part is I1
       | If A == I1 : Update part is I0



Customization
=============


Parameters
----------

First, create a new directory to hold your own configuration. 

For example, one level upper from OpenWize-Up directory :

.. code-block:: bash

   cd OpenWize-Up
   mkdir ../my_custom
   mkdir ../my_custom/cfg

So, we have the following hierarchy :

::
 
   ..  
   ├── my_custom
   │   └── cfg
   └── OpenWize-Up
       ├── sources
       ├── ...
       └── CMakeLists.txt

Modify the default parameters values
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Copy/paste all parameter files that must be customized.

For example, the LANParams.xml :

.. code-block:: bash

   cp ./sources/app/cfg/LANParams.xml ../my_custom/cfg/.

Apply your changes to the LANParams.xml file in "my_custom/cfg" directory. 
Then, rebuild with "GENERATE_PARAM" option enabled :

.. code-block:: bash

   cd _build
   cmake -DCUSTOM_DIR=../my_custom -DGENERATE_PARAM=ON -DCMAKE_BUILD_TYPE=Debug ../. 

.. Note::
   The CUSTOM_DIR is a relative path to the OpenWize-Up main directory.

At this point some .c files have been generated in "my_custom/gen" :

::
 
   my_custom
   ├── cfg
   │   └── LANParams.xml
   └── gen
       ├── parameters_cfg.c
       ├── parameters_cfg.h
       └── parameters_default.c

.. Note::
   The GENERATE_PARAM option is required each time one parameter xml file is modified
   and required to be regenerated. 
   So, once the parameters ".c" files have been generated, the GENERATE_PARAM option is
   not longer required.

.. warning:: 
   The option "-DGENERATE_PARAM=ON" requires that the cmake command line is 
   launch 2 times. This is due to a known bug and will be fixed in a future version.


Add your own parameter file
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Create an xml file named "BusinessParams.xml" in "my_custom/cfg" directory, 
then write the definition of your own parameters. 

Then, rebuild with "GENERATE_PARAM" option enabled :

.. code-block:: bash

   cd _build
   cmake -DCUSTOM_DIR=../my_custom \
         -DGENERATE_PARAM=ON \
         -DCMAKE_BUILD_TYPE=Debug \
         ../. 

The ".c" files have been generated in "my_custom/gen" :

::
 
   my_custom
   ├── cfg
   │   └── BusinessParams.xml
   └── gen
       ├── parameters_cfg.c
       ├── parameters_cfg.h
       └── parameters_default.c


The default name is "BusinessParams.xml" but you may rename it with the 
CUSTOM_BUSINESS_XML command line option. 

For example : "-DCUSTOM_BUSINESS_XML=MyBusinessParams.xml"

Then, rebuild with "GENERATE_PARAM" option enabled :

.. code-block:: bash

   cd _build
   cmake -DCUSTOM_DIR=../my_custom \
         -DCUSTOM_BUSINESS_XML=MyBusinessParams.xml \
         -DGENERATE_PARAM=ON \
         -DCMAKE_BUILD_TYPE=Debug \
         ../. 

The ".c" files have been generated in "my_custom/gen" :

::
 
   ../../my_custom
   ├── cfg
   │   └── MyBusinessParams.xml
   └── gen
       ├── parameters_cfg.c
       ├── parameters_cfg.h
       └── parameters_default.c



Change the application binary file name
---------------------------------------

The default application binary file name is "App_WizeUp" and may be redefined with 
CUSTOM_BIN_NAME command line option. 

For example : "-DCUSTOM_BIN_NAME=custom_app"

.. code-block:: bash

   cd _build
   cmake -DCUSTOM_BIN_NAME=custom_app \
         -DCMAKE_BUILD_TYPE=Debug \
         ../. 


Redefine the hardware information
---------------------------------

Hardware information that define the board is the one given by the ATI command. 

These are hard-coded in the application FW.

.. list-table:: 
   :widths: 20 50 20 20 20
   :header-rows: 1
   
   * - Designation
     - Description
     - Cmake variable
     - Format
     - Default
   * - Name
     - This is the name of the board
     - HW_NAME
     - String
     - WIZEUP
   * - Vendor
     - The board manufacturer or vendor
     - HW_VENDOR
     - String
     - ALCIOM
   * - Model
     - This define the board model or type 
     - HW_MODEL
     - String
     - WZ1000
   * - Major
     - The board major version number 
     - HW_VER_MAJ
     - Integer
     - 1
   * - Minor
     - The board minor version number 
     - HW_VER_MIN
     - Integer
     - 8
   * - Revision
     - The board revision number 
     - HW_VER_REV
     - Integer
     - 0
   * - Date
     - The board design or production date 
     - HW_DATE
     - String
     -

.. Note::
   The WizeUp board version number is form by one integer number and 1 letter (e.g. 1C). 
   So, the letter is convert in integer number by its position in the alphabet (e.g. : C become 3).    

For example, we can redefine our hardware as : 

.. code-block:: bash

   cd _build
   cmake -DHW_NAME="MyBoard" \
         -DHW_VENDOR="Vendor" \
         -DHW_MODEL="1stMod" \
         -DHW_VER_MAJ=0 \
         -DHW_VER_MIN=1 \
         -DHW_VER_REV=0 \
         -DHW_DATE="2023-12-12 10:10:10" \
         -DCMAKE_BUILD_TYPE=Debug \
         ../. 

Use a cmake file to customize
-----------------------------

You can use a cmake file to help the customization. However, the file name is 
restricted to "custom-config.cmake" in order to be find by the our cmake framework.
An example is given in "OpenWize-Up/tools/help" directory. The given "custom-config.cmake"
file include most of the customizable options, just uncomment and change them 
in order to meet your requirements. 

First, create a new directory to hold your own configuration. 

For example, one level upper from OpenWize-Up directory :

.. code-block:: bash

   cd OpenWize-Up
   mkdir ../my_custom

Copy/Paste the "custom-config.cmake" :

.. code-block:: bash

   cp ./tools/help/custom-config.cmake ../my_custom/.


Apply your changes, then rebuild it :

.. code-block:: bash

   cd _build
   cmake -DCUSTOM_DIR=../my_custom \
         -DCMAKE_BUILD_TYPE=Debug \
         ../. 

.. Note::
   Depending on your modification, it may required to regenerate the parameters 
   ".c" file (-DGENERATE_PARAM=ON) . 
   

.. *****************************************************************************
.. references

.. _`memory_mapping.txt`: https://github.com/WizeEveryWhere/OpenWize-Up/blob/develop/sources/bootstrap/memory_mapping.txt
.. _`img_gen.sh`: https://github.com/WizeEveryWhere/OpenWize-Up/blob/develop/tools/scripts/img_gen.sh

