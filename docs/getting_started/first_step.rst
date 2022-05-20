
.. ****************************************************************************

First steps
===========

Cloning this repository
-----------------------

::

   git clone --recurse-submodules https://github.com/WizeEveryWhere/OpenWize-Up.git

Prerequitsite
-------------

Before you start, take a look to the ``Requirement`` section at `OpenWize Documentation`_ page.

In order to be able to build the OpenWize'Up, follow the next two steps. This 
process is necessary only the first time.

- Install the STM32Cube IDE :ref:`STM32Cube IDE installation`.
- Setup environment variables as explained in section :ref:`Setup the environement`.


Build the application
---------------------

.. code-block:: bash

   cd OpenWize-Up
   mkdir -p _build
   cd _build

.. code-block:: bash 

   cmake -DCMAKE_BUILD_TYPE=Debug ../. 
   make App_WizeUp -j
   make install
   
:: 

   _install/
      └── bin
          ├── App_WizeUp      : The "elf" file (i.e. with debug symboles)
          ├── App_WizeUp.bin  : Pure binary file
          ├── App_WizeUp.lst  : Disassembly listing
          └── App_WizeUp.map  : Symboles and files mapping


Load and run the firmware
-------------------------

The following paragraphes depicts how to experiment OpenWize'Up. 

What we need : 

- Wize'Up assemble on its base board.
- a probe, see :ref:`for compatible one here <Debug Probe>`.
- A :ref:`Smartbrick <Test tools>` or TRX module. 

In the following we use a Nucleo board as ST-Link debugger.

Three consoles are needs :

- Console A : Use by the Smartbrick to display received / sent frames and other trace information
- Console B : Use by the OpenWize'Up to get AT command and send back its response
- Console C : Use to "control" the OpenWize'Up (i.e. send AT command)

**************

#. Connect the probe to the Wize'Up base board and to your computer
#. Connect the Wize'Up base board to your computer
#. Console A : 
 
   - Launch the *trxServices* script (see banc_lib documentation)
   
      .. code-block:: bash
      
         perl trxServices.pl -t trx.cfg -m main_fast.cfg -vvv

#. Console B :
 
   - Find the Wize'Up USB device (e.g. /dev/ttyUSB0)
   - Open a terminal emulator (e.g. : minicom) on this device
   - Configure it with 115200 bps, 8 bits data, no parity

#. Console C : 

   - Enter in the OpenWize'Up directory

      .. code-block:: bash
      
         cd Openwize-Up   

   - Find the ST-Link probe id

      .. code-block:: bash 
    
         STM32_Programmer_CLI -l
   
   - Get the probe id then load the firmware (replace index=*your_probe_id*)

      .. code-block:: bash
      
         STM32_Programmer_CLI -c port=SWD mode=UR reset=HWrst index=0 -d _install/bin/App_WizeUp.bin 0x08000000
         
      After few seconds, the firmware is loaded onto the board. 

   - Setup the commissioning

      .. code-block:: bash
       
         source ./tools/scripts/help_AT.sh /dev/ttyUSB0
         SendCommissioning

   - Try request for "PING/PONG"

      .. code-block:: bash
      
         SendAt 'ATPING'

      .. figure:: ../pics/Console_WizeUp_PinPong.png
         :width: 88 %
         :align: center
      
         Console B : OpenWize'Up on PING/PONG.


      .. figure:: ../pics/Console_trxService_PingPong.png
         :width: 100 %
         :align: center
      
         Console A : trxServices on PING/PONG.

   - Try request for send "DATA"

      .. code-block:: bash
      
         SendAt 'ATSEND=$F0,$111213141516'

      .. figure:: ../pics/Console_WizeUp_DataCommandResponse.png
         :width: 88 %
         :align: center
      
         Console B : OpenWize'Up on DATA with received COMMAND and send RESPONSE.


      .. figure:: ../pics/Console_trxService_DataCommandResponse.png
         :width: 100 %
         :align: center
      
         Console A : trxServices on DATA with received COMMAND and send RESPONSE.



Use the STM32Cube IDE
---------------------

Import the OpenWize'Up project
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To import the OpenWize'Up project in IDE follow this :ref:`Appendix <Import OpenWize-Up project>` tutorial.

Build the OpenWize'Up application
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To build OpenWize'Up from IDE follow this :ref:`Appendix <Build the OpenWize-Up>` tutorial.

Launch a Debug session
^^^^^^^^^^^^^^^^^^^^^^

To create and launch a debug configuration follow this :ref:`Appendix <Create a Debug Configuration>` tutorial.


.. *****************************************************************************
.. references

.. only:: comment
   .. _`OpenWize Documentation`: https://github.com/WizeEveryWhere/OpenWize/blob/main/docs/OpenWize.rst
.. _`OpenWize Documentation`: https://wizeeverywhere-openwize.readthedocs.io/en/latest
.. _`Alciom` : https://www.alciom.com/en/home
.. _`STM32CubeIDE`: https://www.st.com/en/development-tools/stm32cubeide.html#get-software
