#!/bin/bash 

# Default options
PROG_TOOL=`which STM32_Programmer_CLI`;

APP_DIR="../_install/bin";

#cmd="STM32_Programmer_CLI -c port=SWD mode=UR reset=HWrst";
cmd="STM32_Programmer_CLI -c port=SWD mode=UR reset=SWrst";

function usage(){
    echo "Usage :"
    echo "$(basename $0) prog_dir probe_id";
    echo "STM32_Programmer_CLI -l ; to get the probe_id" 
}

function do_it(){
    local idx=$1;
    
    ${cmd} index=$idx --skipErase --erase all -d ${IMG_0_BIN} ${IMG_0_ADDRESS} -d ${IMG_1_BIN} ${IMG_1_ADDRESS} -d ${BOOT_BIN} ${BOOT_ADDRESS};
    sleep 1;
    ${cmd} index=$idx -hardRst;
    #${cmd} index=$idx -run;
    #sleep 1;
    echo "help : ${cmd} index=$idx -hardRst";
}

if [[ "${PROG_TOOL}" == "" ]]
then
    echo "Error! It seems that the \"STM32_Programmer_CLI\" program doesn't exist.";
    echo "Ensure your set environement variable (see the main \"README.md\" and/or the \"set_env.sh\" script.";
    exit;
fi


APP_DIR=$1;
if [[ "${APP_DIR}" == "" ]]
then 
    echo "Error! Give an application directory where to find binary files.";
    usage;
    exit;
fi

if [[ ! -d "${APP_DIR}" ]]
then 
    echo "Error! The given directory \"${APP_DIR}\" doesn't exist1.";
    exit;
fi

PROBE_INDEX=$2
if [[ "${PROBE_INDEX}" == "" ]]
then 
    echo "Error! Give a probe index to be able to continu.";
    echo "Available devices :"
    ${PROG_TOOL} -l #st-link
    exit;
fi

BOOT_BIN=${APP_DIR}"/bootstrap.bin";
IMG_0_BIN=${APP_DIR}"/img_0.bin"
IMG_1_BIN=${APP_DIR}"/img_1.bin"

BOOT_ADDRESS=0x08000000
IMG_0_ADDRESS=0x0802c000
IMG_1_ADDRESS=0x08056000

#show_default;
do_it "${PROBE_INDEX}";
