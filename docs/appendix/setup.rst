.. ****************************************************************************

.. _Setup the environment:

Setup the environment
=====================

Environment variables :

- CROSS_TOOL_PATH : give the main path of the cross-toolchain, which is used and
  mandatory by the cmake build system.
- ST_PROG_PATH : give the main path of the STM32Cube programmer tool. This tool
  is use to upload the binary firmware on the target board from command line, so
  not required with the IDE.
- JLINK_PROG_PATH : give the main path of the Segger programmer tool. This tool
  is use to upload the binary firmware on the target board from command line, so
  not required with the IDE.

A script is provided to set them for you.
In a console, execute the following : 

:: 
   
   export PATH=$PATH:$(pwd)/third-party/.OpenWize/tools/scripts;
   ./set_env.sh -i 

The script will show you something like : 

:: 

   Installing environment variable into bashrc
   Set the STM32Cube installation main path : (expl: /opt/Application/st/stm32cubeide_1.8.0/plugins)
   Path : 
   
Check the given example path and change it if required. Else hit ENTER.

At the end, answer yes 

::

   Installing environment variable into bashrc
   Set the STM32Cube installation main path : (expl: /opt/Application/st/stm32cubeide_1.8.0/plugins)
   Path : 
   STM32Cube path set to default : /opt/Application/st/stm32cubeide_1.8.0/plugins
   
   Found : /opt/Application/st/stm32cubeide_1.8.0/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.9-2020-q2-update.linux64_2.0
   Found : /opt/Application/st/stm32cubeide_1.8.0/plugins/com.st.stm32cube.ide.mcu.externaltools.cubeprogrammer.linux64_2.0.100.202110141430/tool
   Found : /opt/Application/st/stm32cubeide_1.8.0/plugins/com.st.stm32cube.ide.mcu.externaltools.jlink.linux64_2.0.100.202110251254/tools/bin
   Do you want to install in your environment variable (bashrc) (recommended)
   Answer (yes or no): yes

Now, each new console will have these environment variables automatically set. 

